#pragma once
#include <JuceHeader.h>
#include "PadTable.h"

//==============================================================================
//  MC Synth — 120 sounds across five kits, a 16-step sequencer, and the
//  BRIGHT / LO-FI / SPACE controls from the web version.
//
//  Audio path (mirrors the web app so both sound the same):
//     oscillators -> per-voice lowpass with its own envelope -> ADSR gain
//        -> master (BRIGHT/LO-FI bypassable crusher) -> compressor -> limiter
//==============================================================================

struct PadParams
{
    Wave  wave = Wave::square;
    float freq = 440.0f;
    float dur  = 0.30f;
    float vol  = 0.5f;
    float q    = 0.0f;
    float glide = 1.0f;
    float detune = 1.0f;
    bool  soft = false;
    bool  low  = false;
    bool  wob  = false;
};

//==============================================================================
class MCSynthSound : public juce::SynthesiserSound
{
public:
    MCSynthSound() = default;
    bool appliesToNote (int) override    { return true; }
    bool appliesToChannel (int) override { return true; }
};

//==============================================================================
class MCSynthVoice : public juce::SynthesiserVoice
{
public:
    MCSynthVoice() = default;

    bool canPlaySound (juce::SynthesiserSound* s) override
    {
        return dynamic_cast<MCSynthSound*> (s) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int) override;
    void stopNote (float, bool allowTailOff) override;
    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override {}

    void renderNextBlock (juce::AudioBuffer<float>&, int startSample, int numSamples) override;

    // ---- engine settings, pushed from the processor each block ----
    void setGlobal (float bright, float crush, float space, float gain);
    // ---- which pad recipe this voice should play (nullptr = pitched synth) ----
    void setPad (const Pad* pad)                    { padRecipe = pad; }
    void setPitched (bool shouldPitch)              { pitched = shouldPitch; }
    void setBaseFreq (float hz)                     { baseFrequency = hz; }

private:
    const Pad* padRecipe = nullptr;
    bool  pitched = false;

    float level = 0.0f;
    float baseFrequency = 440.0f;
    float pitchBend = 1.0f;

    // oscillator state
    double phase1 = 0.0, phase2 = 0.0, lfoPhase = 0.0;
    juce::ADSR adsr;

    // filter state
    juce::dsp::StateVariableTPTFilter<float> filterL, filterR;

    // bitcrusher state
    float holdL = 0.0f, holdR = 0.0f;
    int   holdCount = 0;
    juce::Random rng;

    // cached globals
    float gBright = 0.62f, gCrush = 0.16f, gSpace = 0.22f, gGain = 0.9f;

    // noise source for the noise-based pads
    juce::Random noiseRng;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MCSynthVoice)
};

//==============================================================================
//  A simple 16-step sequencer, one lane per kit pad slot.
//==============================================================================
class StepSequencer
{
public:
    static constexpr int kSteps = 16;
    static constexpr int kLanes = 4;

    void prepare (double sr);
    void reset();

    void setPlaying (bool on);
    bool isPlaying() const            { return playing; }

    void setBpm (float bpm);
    float getBpm() const              { return bpm; }

    void setNumSteps (int n)          { numSteps = juce::jlimit (4, kSteps, n); }
    int  getNumSteps() const          { return numSteps; }

    void setStep (int lane, int step, int padIndexInKit);
    int  getStep (int lane, int step) const;
    void clear();
    void clearLane (int lane);

    // Fills `hitLane` with the lane indices that fire on this step,
    // and `hitPad` with the pad index in the current kit for each.
    void collect (int step, std::array<int, kLanes>& hitLane,
                  std::array<int, kLanes>& hitPad, int& count) const;

    // Called from the audio thread. Returns the number of steps that were due.
    int advance (double blockStartTime, double blockEndTime,
                 std::array<int, 64>& dueSteps);

    int getCurrentStep() const        { return displayStep; }

private:
    bool  playing = false;
    float bpm = 110.0f;
    int   numSteps = 16;
    int   pattern[kLanes][kSteps];
    double sampleRate = 44100.0;
    double nextStepTime = 0.0;
    int   nextStepIndex = 0;
    int   displayStep = 0;

    double stepSeconds() const { return 60.0 / (double) bpm / 4.0; }   // 16th notes
};

//==============================================================================
class MCSynthAudioProcessor : public juce::AudioProcessor
{
public:
    MCSynthAudioProcessor();
    ~MCSynthAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout&) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "MC Synth"; }
    bool acceptsMidi() const override  { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return "Default"; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    // ---- the editor drives these ----
    StepSequencer& sequencer()                       { return seq; }
    int  getKitIndex() const                         { return (int) kitParam->get(); }
    void setKitIndex (int i);
    void triggerPad (int padIndexInKit, float velocity = 1.0f);
    int  getLastTriggeredPad() const                 { return lastPad; }
    void setLastTriggeredPad (int i)                 { lastPad = i; }

    // so the editor can light up steps as they fire
    std::atomic<int> uiStep { -1 };

    juce::AudioProcessorValueTreeState apvts;

    juce::AudioParameterFloat* brightParam = nullptr;
    juce::AudioParameterFloat* crushParam  = nullptr;
    juce::AudioParameterFloat* spaceParam  = nullptr;
    juce::AudioParameterFloat* gainParam   = nullptr;
    juce::AudioParameterChoice* kitParam   = nullptr;
    juce::AudioParameterFloat* bpmParam    = nullptr;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();

    juce::Synthesiser synth;
    StepSequencer seq;
    int lastPad = 0;

    // master chain
    juce::dsp::Compressor<float> comp;
    juce::dsp::Limiter<float> limiter;
    juce::AudioBuffer<float> compBuffer;

    juce::AudioParameterFloat* attackParam  = nullptr;
    juce::AudioParameterFloat* releaseParam = nullptr;
    juce::AudioParameterFloat* toneParam    = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MCSynthAudioProcessor)
};
