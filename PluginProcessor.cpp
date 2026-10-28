#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//  Voice
//==============================================================================
void MCSynthVoice::setGlobal (float bright, float crush, float space, float gain)
{
    gBright = bright;
    gCrush  = crush;
    gSpace  = space;
    gGain   = gain;

    // the crusher quantises in time; a smaller hold is a cleaner sound
    const int hold = juce::jlimit (1, 32, (int) (1.0f + (1.0f - crush) * 31.0f));
    holdCount = 0;
    juce::ignoreUnused (hold);
}

void MCSynthVoice::startNote (int midiNoteNumber, float velocity,
                              juce::SynthesiserSound*, int)
{
    baseFrequency = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    phase1 = phase2 = lfoPhase = 0.0;
    level = velocity;
    adsr.noteOn();
}

void MCSynthVoice::stopNote (float, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        adsr.reset();
        clearCurrentNote();
    }
}

void MCSynthVoice::pitchWheelMoved (int newValue)
{
    const float norm = (float) (newValue - 8192) / 8192.0f;
    pitchBend = std::pow (2.0f, norm * 2.0f / 12.0f);
}

void MCSynthVoice::renderNextBlock (juce::AudioBuffer<float>& out,
                                    int startSample, int numSamples)
{
    if (! isVoiceActive())
        return;

    // ---- pull the recipe: either a kit pad, or the plain pitched synth ----
    PadParams P;
    if (padRecipe != nullptr)
    {
        P.wave   = padRecipe->wave;
        P.freq   = padRecipe->freq;
        P.dur    = padRecipe->dur;
        P.vol    = padRecipe->vol;
        P.q      = padRecipe->q;
        P.glide  = padRecipe->glide;
        P.detune = padRecipe->detune;
        P.soft   = padRecipe->soft;
        P.low    = padRecipe->low;
        P.wob    = padRecipe->wob;
    }

    const double sr = getSampleRate();

    // ---- envelope: bowed pads ramp in, hits are instant ----
    const float rel = P.dur;
    const float atk = P.soft ? juce::jlimit (0.02f, 0.22f, rel * 0.16f) : 0.004f;
    adsr.setParameters ({ atk, 0.02f, 1.0f, rel });

    // ---- BRIGHT sets the filter cutoff; deep pads stay dark ----
    float brightMul = 0.35f + gBright * 3.6f;
    if (P.low) brightMul = juce::jmin (brightMul, 1.15f);
    const float cutoff = juce::jlimit (80.0f, 12000.0f,
                                       (pitched ? baseFrequency * 5.5f : P.freq * 5.5f) * brightMul);
    const float reso = P.q > 0.0f ? juce::jlimit (0.5f, 12.0f, P.q) : 0.9f;

    filterL.setCutoffFrequency (cutoff);
    filterR.setCutoffFrequency (cutoff);
    filterL.setResonance (reso);
    filterR.setResonance (reso);

    // ---- noise pads read from a shared buffer prepared in prepareToPlay ----
    const bool isNoise = (P.wave == Wave::noise);

    float* left  = out.getWritePointer (0, startSample);
    float* right = out.getNumChannels() > 1 ? out.getWritePointer (1, startSample) : nullptr;

    for (int i = 0; i < numSamples; ++i)
    {
        const float env = adsr.getNextSample();

        float s1 = 0.0f, s2 = 0.0f;

        if (isNoise)
        {
            s1 = noiseRng.nextFloat() * 2.0f - 1.0f;
            s2 = noiseRng.nextFloat() * 2.0f - 1.0f;
        }
        else
        {
            // the pad's own frequency when it is a kit pad, else the MIDI note
            float f = pitched ? baseFrequency * pitchBend : P.freq;
            if (P.glide > 1.001f) f *= P.glide;      // start sharp, then settle
            if (P.wob)
            {
                lfoPhase += 6.5 / sr;
                if (lfoPhase >= 1.0) lfoPhase -= 1.0;
                f *= 1.0f + (float) std::sin (lfoPhase * juce::MathConstants<double>::twoPi) * 0.035f;
            }

            const double inc1 = f / sr;
            const double inc2 = f * P.detune / sr;

            phase1 += inc1; if (phase1 >= 1.0) phase1 -= 1.0;
            phase2 += inc2; if (phase2 >= 1.0) phase2 -= 1.0;

            auto oscAt = [] (Wave w, double ph)
            {
                const double t = ph;
                switch (w)
                {
                    case Wave::sine:     return (float) std::sin (t * juce::MathConstants<double>::twoPi);
                    case Wave::square:   return t < 0.5 ? 1.0f : -1.0f;
                    case Wave::saw:      return (float) (2.0 * t - 1.0);
                    case Wave::triangle: return (float) (1.0 - 4.0 * std::abs (t - 0.5));
                    default:             return 0.0f;
                }
            };

            s1 = oscAt (P.wave, phase1);
            if (P.detune > 1.001f) s1 = s1 * 0.6f + oscAt (P.wave, phase2) * 0.4f;
            s2 = s1;
        }

        // per-voice filter
        float fl = filterL.processSample (0, s1);
        float fr = filterR.processSample (0, s2);

        float a = fl * env * level * P.vol * gGain;
        float b = fr * env * level * P.vol * gGain;

        // ---- LO-FI: quantise amplitude, and add a whisper of noise ----
        if (gCrush > 0.01f)
        {
            const float bits  = 16.0f - gCrush * 12.0f;      // 16 down to 4
            const float steps = std::pow (2.0f, bits);
            a = std::round (a * steps) / steps;
            b = std::round (b * steps) / steps;
            a += (rng.nextFloat() - 0.5f) * gCrush * 0.006f;
            b += (rng.nextFloat() - 0.5f) * gCrush * 0.006f;
        }

        left[i]  += a;
        if (right != nullptr) right[i] += b;

        if (! adsr.isActive())
        {
            clearCurrentNote();
            break;
        }
    }

    juce::ignoreUnused (startSample);
}

//==============================================================================
//  StepSequencer
//==============================================================================
void StepSequencer::prepare (double sr)
{
    sampleRate = sr;
    clear();
    reset();
}

void StepSequencer::reset()
{
    nextStepTime = 0.0;
    nextStepIndex = 0;
    displayStep = 0;
}

void StepSequencer::setPlaying (bool on)
{
    playing = on;
    if (on) reset();
}

void StepSequencer::setBpm (float b)
{
    bpm = juce::jlimit (40.0f, 240.0f, b);
}

void StepSequencer::setStep (int lane, int step, int padIndexInKit)
{
    if (lane < 0 || lane >= kLanes || step < 0 || step >= kSteps) return;
    pattern[lane][step] = padIndexInKit;
}

int StepSequencer::getStep (int lane, int step) const
{
    if (lane < 0 || lane >= kLanes || step < 0 || step >= kSteps) return -1;
    return pattern[lane][step];
}

void StepSequencer::clear()
{
    for (int l = 0; l < kLanes; ++l)
        for (int s = 0; s < kSteps; ++s)
            pattern[l][s] = -1;
}

void StepSequencer::clearLane (int lane)
{
    if (lane < 0 || lane >= kLanes) return;
    for (int s = 0; s < kSteps; ++s) pattern[lane][s] = -1;
}

void StepSequencer::collect (int step, std::array<int, kLanes>& hitLane,
                             std::array<int, kLanes>& hitPad, int& count) const
{
    count = 0;
    if (step < 0 || step >= kSteps) return;
    for (int l = 0; l < kLanes; ++l)
    {
        const int p = pattern[l][step];
        if (p >= 0 && count < kLanes)
        {
            hitLane[count] = l;
            hitPad[count]  = p;
            ++count;
        }
    }
}

// Called once per audio block.  This is the timing core: every step time is
// accumulated from the previous one, so the tempo can never drift.
int StepSequencer::advance (double blockStartTime, double blockEndTime,
                            std::array<int, 64>& dueSteps)
{
    if (! playing) return 0;

    const double dur = stepSeconds();
    if (nextStepTime < blockStartTime)
        nextStepTime = blockStartTime;          // first block, or after a stall

    int n = 0;
    while (nextStepTime < blockEndTime && n < 64)
    {
        dueSteps[(size_t) n++] = nextStepIndex;
        nextStepTime += dur;
        nextStepIndex = (nextStepIndex + 1) % numSteps;
    }
    if (n > 0) displayStep = dueSteps[(size_t) (n - 1)];
    return n;
}

//==============================================================================
//  Processor
//==============================================================================
MCSynthAudioProcessor::MCSynthAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMS", createLayout())
{
    synth.addSound (new MCSynthSound());
    for (int i = 0; i < 16; ++i)
        synth.addVoice (new MCSynthVoice());

    brightParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("bright"));
    crushParam  = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("crush"));
    spaceParam  = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("space"));
    gainParam   = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("gain"));
    kitParam    = dynamic_cast<juce::AudioParameterChoice*> (apvts.getParameter ("kit"));
    bpmParam    = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("bpm"));
    attackParam = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("attack"));
    releaseParam= dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("release"));
    toneParam   = dynamic_cast<juce::AudioParameterFloat*> (apvts.getParameter ("tone"));
}

juce::AudioProcessorValueTreeState::ParameterLayout MCSynthAudioProcessor::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    juce::StringArray kitNames;
    for (int i = 0; i < NUM_KITS; ++i) kitNames.add (KIT_NAMES[i]);

    layout.add (std::make_unique<juce::AudioParameterChoice> ("kit", "Kit", kitNames, 4));  // EPIC KIT

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "bright", "Bright", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.62f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "crush", "Lo-Fi", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.16f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "space", "Space", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.22f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "gain", "Gain", juce::NormalisableRange<float> (0.0f, 1.5f, 0.001f), 0.85f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "bpm", "BPM", juce::NormalisableRange<float> (60.0f, 190.0f, 0.5f), 110.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "attack", "Attack", juce::NormalisableRange<float> (0.001f, 2.0f, 0.001f, 0.35f), 0.005f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "release", "Release", juce::NormalisableRange<float> (0.01f, 4.0f, 0.001f, 0.35f), 0.25f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "tone", "Tone", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.55f));

    return layout;
}

void MCSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    seq.prepare (sampleRate);

    comp.prepare ({ sampleRate, (juce::uint32) samplesPerBlock, 2 });
    comp.setThreshold (-14.0f);
    comp.setRatio (5.0f);
    comp.setAttack (4.0f);
    comp.setRelease (180.0f);

    limiter.prepare ({ sampleRate, (juce::uint32) samplesPerBlock, 2 });
    limiter.setThreshold (-3.0f);
    limiter.setRelease (100.0f);

    compBuffer.setSize (2, samplesPerBlock);
}

bool MCSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

void MCSynthAudioProcessor::setKitIndex (int i)
{
    if (kitParam != nullptr)
        kitParam->setValueNotifyingHost (kitParam->convertTo0to1 ((float) juce::jlimit (0, NUM_KITS - 1, i)));
}

void MCSynthAudioProcessor::triggerPad (int padIndexInKit, float velocity)
{
    const int k = getKitIndex();
    if (k < 0 || k >= NUM_KITS) return;
    const int p = juce::jlimit (0, PADS_PER_KIT - 1, padIndexInKit);

    // a pad is played by a dedicated voice whose recipe is set before it starts
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* v = dynamic_cast<MCSynthVoice*> (synth.getVoice (i)))
        {
            if (! v->isVoiceActive())
            {
                v->setPad (&KITS[k][p]);
                v->setPitched (false);
                v->setBaseFreq (KITS[k][p].freq);
                v->startNote (60, velocity, nullptr, 0);
                lastPad = p;
                return;
            }
        }
    }
}

void MCSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();
    buffer.clear();

    const float bright = brightParam ? brightParam->get() : 0.62f;
    const float crush  = crushParam  ? crushParam->get()  : 0.16f;
    const float space  = spaceParam  ? spaceParam->get()  : 0.22f;
    const float gain   = gainParam   ? gainParam->get()   : 0.85f;
    const float bpm    = bpmParam    ? bpmParam->get()    : 110.0f;

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = dynamic_cast<MCSynthVoice*> (synth.getVoice (i)))
            v->setGlobal (bright, crush, space, gain);

    // ---- MIDI notes use the plain pitched synth voice ----
    for (const auto meta : midi)
    {
        const auto m = meta.getMessage();
        if (m.isNoteOn())
        {
            for (int i = 0; i < synth.getNumVoices(); ++i)
            {
                if (auto* v = dynamic_cast<MCSynthVoice*> (synth.getVoice (i)))
                {
                    if (! v->isVoiceActive())
                    {
                        v->setPad (nullptr);
                        v->setPitched (true);
                        v->startNote (m.getNoteNumber(), m.getFloatVelocity(), nullptr, 0);
                        break;
                    }
                }
            }
        }
        else if (m.isNoteOff())
        {
            for (int i = 0; i < synth.getNumVoices(); ++i)
            {
                auto* v = synth.getVoice (i);
                if (v->getCurrentlyPlayingNote() == m.getNoteNumber() && v->isVoiceActive())
                    v->stopNote (0.0f, true);
            }
        }
    }

    // ---- the sequencer: schedule the steps due inside this block ----
    seq.setBpm (bpm);
    std::array<int, 64> due {};
    // AudioPlayHead gives the real stream time; fall back to a running count.
    double blockStart = 0.0, blockEnd = 0.0;
    if (auto* ph = getPlayHead())
    {
        if (auto pos = ph->getPosition())
        {
            const double t = pos->getTimeInSeconds().orFallback (0.0);
            blockStart = t;
            blockEnd = t + (double) numSamples / getSampleRate();
        }
    }

    const int dueCount = seq.advance (blockStart, blockEnd, due);
    for (int d = 0; d < dueCount; ++d)
    {
        std::array<int, StepSequencer::kLanes> lane {}, pad {};
        int count = 0;
        seq.collect (due[(size_t) d], lane, pad, count);
        for (int c = 0; c < count; ++c)
        {
            triggerPad (pad[(size_t) c], 1.0f);
            uiStep.store (due[(size_t) d]);      // the editor lights this up
        }
    }

    synth.renderNextBlock (buffer, midi, 0, numSamples);

    // ---- master chain ----
    compBuffer.makeCopyOf (buffer, true);
    juce::dsp::AudioBlock<float> block (compBuffer);
    comp.process (juce::dsp::ProcessContextReplacing<float> (block));
    limiter.process (juce::dsp::ProcessContextReplacing<float> (block));
    buffer.makeCopyOf (compBuffer, true);
}

void MCSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ValueTree state = apvts.copyState();

    // patterns and lane assignments live outside the parameters, so store them here
    juce::ValueTree pat ("PATTERN");
    pat.setProperty ("steps", seq.getNumSteps(), nullptr);
    for (int l = 0; l < StepSequencer::kLanes; ++l)
        for (int s = 0; s < StepSequencer::kSteps; ++s)
            pat.setProperty ("s" + juce::String (l) + "_" + juce::String (s), seq.getStep (l, s), nullptr);
    state.addChild (pat, -1, nullptr);

    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void MCSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName (apvts.state.getType()))
        {
            auto tree = juce::ValueTree::fromXml (*xml);
            auto pat = tree.getChildWithName ("PATTERN");
            if (pat.isValid())
            {
                seq.setNumSteps ((int) pat.getProperty ("steps", 16));
                for (int l = 0; l < StepSequencer::kLanes; ++l)
                    for (int s = 0; s < StepSequencer::kSteps; ++s)
                        seq.setStep (l, s, (int) pat.getProperty ("s" + juce::String (l) + "_" + juce::String (s), -1));
                tree.removeChild (pat, nullptr);
            }
            apvts.replaceState (tree);
        }
    }
}

//==============================================================================
juce::AudioProcessorEditor* MCSynthAudioProcessor::createEditor()
{
    return new MCSynthAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MCSynthAudioProcessor();
}
