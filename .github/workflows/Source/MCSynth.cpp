// ============================================================================
//  MC Synth — one single source file. Nothing else is needed.
//  120 sounds across 5 kits, a 16-step machine, and the master controls.
// ============================================================================

#include <JuceHeader.h>
#include <array>

#include <array>

// ============================================================================
//  Pad table — the same 120 sounds as the web app, five kits of 24.
//  Generated from the web version so both instruments sound identical.
// ============================================================================

enum class Wave { sine, square, saw, triangle, noise };

struct Pad
{
    const char* name;
    Wave        wave;
    float       freq;      // Hz
    float       dur;       // seconds (release length)
    float       vol;       // 0..1
    float       q;         // filter resonance, 0 = default
    float       glide;     // start-pitch multiplier, 1 = none
    float       detune;    // 1 = off; e.g. 1.005 = one detuned copy
    bool        soft;      // slow attack (bowed / swelled)
    bool        low;       // keep the filter dark, for deep sounds
    bool        wob;       // add vibrato
};

static const Pad kitBLOCK[] = {
    { "Grass", Wave::noise, 1400.0000f, 0.10f, 0.50f, 1.0f, 1.00f, 1.000f, false, false, false },
    { "Dirt", Wave::noise, 700.0000f, 0.12f, 0.55f, 2.2f, 1.00f, 1.000f, false, false, false },
    { "Stone", Wave::noise, 480.0000f, 0.13f, 0.62f, 3.6f, 1.00f, 1.000f, false, false, false },
    { "Wood", Wave::noise, 900.0000f, 0.11f, 0.58f, 3.8f, 1.00f, 1.000f, false, false, false },
    { "Sand", Wave::noise, 2600.0000f, 0.22f, 0.40f, 0.9f, 1.00f, 1.000f, false, false, false },
    { "Gravel", Wave::noise, 1100.0000f, 0.18f, 0.50f, 1.4f, 1.00f, 1.000f, false, false, false },
    { "Glass", Wave::noise, 3400.0000f, 0.16f, 0.45f, 5.0f, 1.00f, 1.000f, false, false, false },
    { "Wool", Wave::noise, 380.0000f, 0.09f, 0.45f, 1.6f, 1.00f, 1.000f, false, false, false },
    { "Snow", Wave::noise, 2000.0000f, 0.13f, 0.42f, 1.1f, 1.00f, 1.000f, false, false, false },
    { "Leaves", Wave::noise, 1600.0000f, 0.20f, 0.38f, 0.8f, 1.00f, 1.000f, false, false, false },
    { "Nether", Wave::noise, 300.0000f, 0.30f, 0.55f, 1.2f, 1.00f, 1.000f, false, false, false },
    { "Obsidian", Wave::noise, 220.0000f, 0.22f, 0.60f, 3.0f, 1.00f, 1.000f, false, false, false },
    { "Portal", Wave::sine, 170.0000f, 0.85f, 0.48f, 0.0f, 2.60f, 1.010f, false, false, false },
    { "Anvil", Wave::noise, 260.0000f, 0.45f, 0.65f, 4.5f, 1.00f, 1.000f, false, false, false },
    { "Chest", Wave::noise, 640.0000f, 0.20f, 0.52f, 2.8f, 1.00f, 1.000f, false, false, false },
    { "Door", Wave::noise, 520.0000f, 0.26f, 0.50f, 2.4f, 1.00f, 1.000f, false, false, false },
    { "Lever", Wave::noise, 1200.0000f, 0.07f, 0.45f, 6.0f, 1.00f, 1.000f, false, false, false },
    { "Button", Wave::noise, 2400.0000f, 0.05f, 0.50f, 8.0f, 1.00f, 1.000f, false, false, false },
    { "Fire", Wave::noise, 800.0000f, 0.35f, 0.42f, 0.6f, 1.00f, 1.000f, false, false, false },
    { "Water", Wave::noise, 1500.0000f, 0.28f, 0.40f, 0.7f, 1.00f, 1.000f, false, false, false },
    { "Lava", Wave::noise, 260.0000f, 0.55f, 0.45f, 0.5f, 1.00f, 1.000f, false, false, false },
    { "Fuse", Wave::noise, 3200.0000f, 0.09f, 0.40f, 7.0f, 1.00f, 1.000f, false, false, false },
    { "XP", Wave::triangle, 784.0000f, 0.24f, 0.48f, 0.0f, 1.60f, 1.000f, false, false, false },
    { "Ding", Wave::triangle, 1174.0000f, 0.70f, 0.45f, 0.0f, 1.00f, 1.000f, false, false, false },
};

static const Pad kitBLIP[] = {
    { "Select", Wave::square, 880.0000f, 0.09f, 0.50f, 0.0f, 1.50f, 1.000f, false, false, false },
    { "Click", Wave::square, 1174.0000f, 0.06f, 0.45f, 0.0f, 1.30f, 1.000f, false, false, false },
    { "Confirm", Wave::square, 659.0000f, 0.16f, 0.52f, 0.0f, 1.40f, 1.000f, false, false, false },
    { "Back", Wave::square, 494.0000f, 0.14f, 0.50f, 0.0f, 0.80f, 1.000f, false, false, false },
    { "Error", Wave::square, 196.0000f, 0.22f, 0.55f, 0.0f, 0.70f, 1.000f, false, false, false },
    { "Pop", Wave::square, 1046.0000f, 0.08f, 0.45f, 0.0f, 1.80f, 1.000f, false, false, false },
    { "Blip Hi", Wave::square, 1318.0000f, 0.07f, 0.45f, 0.0f, 1.50f, 1.000f, false, false, false },
    { "Blip Mid", Wave::square, 784.0000f, 0.09f, 0.48f, 0.0f, 1.40f, 1.000f, false, false, false },
    { "Blip Lo", Wave::square, 523.0000f, 0.12f, 0.48f, 0.0f, 1.30f, 1.000f, false, false, false },
    { "Jump", Wave::square, 440.0000f, 0.16f, 0.50f, 0.0f, 2.20f, 1.000f, false, false, false },
    { "Hurt", Wave::square, 440.0000f, 0.26f, 0.55f, 0.0f, 0.55f, 1.000f, false, false, false },
    { "Oof", Wave::square, 220.0000f, 0.30f, 0.58f, 0.0f, 0.60f, 1.000f, false, false, false },
    { "Death", Wave::square, 300.0000f, 0.55f, 0.55f, 0.0f, 0.35f, 1.000f, false, false, false },
    { "Level Up", Wave::triangle, 523.0000f, 0.60f, 0.52f, 0.0f, 2.00f, 1.000f, false, false, false },
    { "Achieve", Wave::triangle, 880.0000f, 0.85f, 0.50f, 0.0f, 1.50f, 1.000f, false, false, false },
    { "Villager", Wave::square, 392.0000f, 0.20f, 0.50f, 0.0f, 1.90f, 1.000f, false, false, true },
    { "Zombie", Wave::square, 147.0000f, 0.42f, 0.52f, 0.0f, 1.10f, 1.000f, false, false, true },
    { "Creeper", Wave::noise, 700.0000f, 0.60f, 0.55f, 1.3f, 1.00f, 1.000f, false, false, false },
    { "Skeleton", Wave::noise, 1800.0000f, 0.12f, 0.45f, 4.0f, 1.00f, 1.000f, false, false, false },
    { "Enderman", Wave::sine, 110.0000f, 0.70f, 0.50f, 0.0f, 2.80f, 1.000f, false, false, true },
    { "Blaze", Wave::noise, 520.0000f, 0.40f, 0.45f, 0.9f, 1.00f, 1.000f, false, false, false },
    { "Ghast", Wave::sine, 330.0000f, 1.10f, 0.45f, 0.0f, 0.50f, 1.000f, false, false, true },
    { "Ender", Wave::sine, 220.0000f, 0.90f, 0.46f, 0.0f, 2.20f, 1.000f, false, false, false },
    { "Totem", Wave::triangle, 1046.0000f, 1.20f, 0.42f, 0.0f, 1.00f, 1.000f, false, false, false },
};

static const Pad kitMELODY[] = {
    { "C", Wave::triangle, 261.6300f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "D", Wave::triangle, 293.6600f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "E", Wave::triangle, 329.6300f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "F", Wave::triangle, 349.2300f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "G", Wave::triangle, 392.0000f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "A", Wave::triangle, 440.0000f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "B", Wave::triangle, 493.8800f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "C2", Wave::triangle, 523.2500f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "D2", Wave::triangle, 587.3300f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "E2", Wave::triangle, 659.2500f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "F2", Wave::triangle, 698.4600f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "G2", Wave::triangle, 783.9900f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "A2", Wave::triangle, 880.0000f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "B2", Wave::triangle, 987.7700f, 0.50f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "C3", Wave::triangle, 1046.5000f, 0.50f, 0.42f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "D3", Wave::triangle, 1174.7000f, 0.50f, 0.42f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "E3", Wave::triangle, 1318.5000f, 0.50f, 0.40f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "F3", Wave::triangle, 1396.9000f, 0.50f, 0.40f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Low C", Wave::square, 130.8100f, 0.70f, 0.50f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Low F", Wave::square, 174.6100f, 0.70f, 0.50f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Low G", Wave::square, 196.0000f, 0.70f, 0.50f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Bell", Wave::triangle, 1567.9800f, 1.40f, 0.36f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Chime", Wave::triangle, 2093.0000f, 1.60f, 0.30f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Pad", Wave::sine, 174.6100f, 2.60f, 0.40f, 0.0f, 1.00f, 1.000f, false, false, false },
};

static const Pad kitMUSIC[] = {
    { "Piano C", Wave::triangle, 261.6300f, 0.80f, 0.50f, 0.0f, 1.00f, 1.000f, true, false, false },
    { "Piano E", Wave::triangle, 329.6300f, 0.80f, 0.50f, 0.0f, 1.00f, 1.000f, true, false, false },
    { "Piano G", Wave::triangle, 392.0000f, 0.80f, 0.50f, 0.0f, 1.00f, 1.000f, true, false, false },
    { "Piano B", Wave::triangle, 493.8800f, 0.80f, 0.50f, 0.0f, 1.00f, 1.000f, true, false, false },
    { "Harp C", Wave::triangle, 523.2500f, 1.10f, 0.44f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Harp E", Wave::triangle, 659.2500f, 1.10f, 0.44f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Harp G", Wave::triangle, 783.9900f, 1.10f, 0.44f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Harp B", Wave::triangle, 987.7700f, 1.10f, 0.44f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Bass", Wave::square, 98.0000f, 0.60f, 0.52f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Bass 5th", Wave::square, 146.8300f, 0.60f, 0.50f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Sub", Wave::sine, 65.4100f, 0.80f, 0.50f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Pluck", Wave::triangle, 440.0000f, 0.22f, 0.46f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Kick", Wave::sine, 120.0000f, 0.24f, 0.62f, 0.0f, 0.35f, 1.000f, true, false, false },
    { "Snare", Wave::noise, 1800.0000f, 0.16f, 0.48f, 1.0f, 1.00f, 1.000f, false, false, false },
    { "Hat", Wave::noise, 6500.0000f, 0.06f, 0.34f, 1.2f, 1.00f, 1.000f, false, false, false },
    { "Tom", Wave::sine, 180.0000f, 0.30f, 0.50f, 0.0f, 0.60f, 1.000f, true, false, false },
    { "Clap", Wave::noise, 1400.0000f, 0.14f, 0.44f, 0.8f, 1.00f, 1.000f, false, false, false },
    { "Shaker", Wave::noise, 5000.0000f, 0.10f, 0.32f, 1.0f, 1.00f, 1.000f, false, false, false },
    { "Cowbell", Wave::square, 587.0000f, 0.20f, 0.42f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Organ", Wave::square, 261.6300f, 1.00f, 0.40f, 0.0f, 1.00f, 1.000f, false, false, false },
    { "Strings", Wave::triangle, 349.2300f, 1.60f, 0.42f, 0.0f, 1.00f, 1.006f, false, false, false },
    { "Choir", Wave::sine, 392.0000f, 1.80f, 0.40f, 0.0f, 1.00f, 1.004f, false, false, false },
    { "Cat", Wave::square, 700.0000f, 0.30f, 0.42f, 0.0f, 1.70f, 1.000f, false, false, true },
    { "Door Creak", Wave::noise, 900.0000f, 0.90f, 0.36f, 6.0f, 1.00f, 1.000f, false, false, false },
};

static const Pad kitEPIC[] = {
    { "Cello Low", Wave::saw, 65.4100f, 2.60f, 0.52f, 0.0f, 1.00f, 1.005f, true, true, false },
    { "Cello 5th", Wave::saw, 98.0000f, 2.60f, 0.50f, 0.0f, 1.00f, 1.005f, true, true, false },
    { "Cello Oct", Wave::saw, 130.8100f, 2.40f, 0.46f, 0.0f, 1.00f, 1.007f, true, true, false },
    { "Bass Drone", Wave::sine, 43.6500f, 3.00f, 0.54f, 0.0f, 1.00f, 1.000f, true, true, false },
    { "Sub Boom", Wave::sine, 32.7000f, 2.20f, 0.52f, 0.0f, 1.00f, 1.000f, true, true, false },
    { "Theme 1", Wave::triangle, 146.8300f, 1.80f, 0.50f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Theme 2", Wave::triangle, 174.6100f, 1.80f, 0.50f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Theme 3", Wave::triangle, 196.0000f, 1.90f, 0.50f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Theme 4", Wave::triangle, 220.0000f, 1.90f, 0.50f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Theme 5", Wave::triangle, 233.0800f, 2.00f, 0.48f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Theme High", Wave::triangle, 293.6600f, 2.00f, 0.44f, 0.0f, 1.00f, 1.006f, true, false, false },
    { "Horn Low", Wave::square, 87.3100f, 1.60f, 0.46f, 0.0f, 1.00f, 1.003f, true, false, false },
    { "Horn Mid", Wave::square, 116.5400f, 1.50f, 0.46f, 0.0f, 1.00f, 1.003f, true, false, false },
    { "Horn Call", Wave::square, 174.6100f, 1.30f, 0.44f, 0.0f, 1.00f, 1.004f, true, false, false },
    { "Horn High", Wave::square, 233.0800f, 1.20f, 0.42f, 0.0f, 1.00f, 1.005f, true, false, false },
    { "Choir Ah", Wave::sine, 220.0000f, 2.40f, 0.44f, 0.0f, 1.00f, 1.003f, true, false, false },
    { "Choir Deep", Wave::sine, 110.0000f, 2.80f, 0.46f, 0.0f, 1.00f, 1.003f, true, true, false },
    { "War Drum", Wave::sine, 70.0000f, 1.10f, 0.62f, 0.0f, 0.55f, 1.000f, true, true, false },
    { "Drum Hit", Wave::sine, 95.0000f, 0.55f, 0.56f, 0.0f, 0.70f, 1.000f, true, true, false },
    { "Timpani", Wave::sine, 98.0000f, 0.90f, 0.52f, 0.0f, 0.80f, 1.000f, true, true, false },
    { "Deep Boom", Wave::noise, 120.0000f, 1.20f, 0.46f, 0.6f, 1.00f, 1.000f, false, true, false },
    { "Riser", Wave::saw, 110.0000f, 1.60f, 0.40f, 0.0f, 4.00f, 1.000f, false, false, true },
    { "Tension", Wave::sine, 82.4100f, 2.80f, 0.42f, 0.0f, 1.00f, 1.009f, true, true, false },
    { "Bell Toll", Wave::triangle, 98.0000f, 2.80f, 0.42f, 0.0f, 1.00f, 1.000f, true, true, false },
};

static const Pad* const KITS[] = { kitBLOCK, kitBLIP, kitMELODY, kitMUSIC, kitEPIC };
static const char* const KIT_NAMES[] = { "BLOCK KIT", "BLIP KIT", "MELODY KIT", "MUSIC KIT", "EPIC KIT" };
static const int NUM_KITS = 5;
static const int PADS_PER_KIT = 24;

//  ---------------------------------------------------------------------------
//  Processor: declarations
//  ---------------------------------------------------------------------------
#include <JuceHeader.h>
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

//  ---------------------------------------------------------------------------
//  Editor: declarations
//  ---------------------------------------------------------------------------
#include <JuceHeader.h>
//==============================================================================
//  The editor: kit selector, a 24-pad grid, four lanes of 16 steps,
//  transport, and the three master knobs.
//==============================================================================
class MCSynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    private juce::Timer
{
public:
    explicit MCSynthAudioProcessorEditor (MCSynthAudioProcessor&);
    ~MCSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    MCSynthAudioProcessor& proc;

    // ---- kit row ----
    juce::ComboBox kitBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> kitAttach;

    // ---- pad grid: 24 pads, 8 across x 3 down ----
    static constexpr int kPadCols = 8;
    static constexpr int kPadRows = 3;
    struct PadButton : public juce::Button
    {
        PadButton() : juce::Button ("pad") {}
        void paintButton (juce::Graphics& g, bool over, bool down) override;
        juce::Colour baseColour;
        juce::String padName;
        int index = 0;
    };
    std::vector<std::unique_ptr<PadButton>> padButtons;

    // ---- lanes: 4 rows x 16 steps ----
    struct StepButton : public juce::Button
    {
        StepButton() : juce::Button ("step") {}
        void paintButton (juce::Graphics& g, bool over, bool down) override;
        int lane = 0, step = 0;
        bool on = false;
        bool isBeat = false;
        bool isCurrent = false;
    };
    std::vector<std::unique_ptr<StepButton>> stepButtons;

    juce::TextButton playButton { "PLAY" };
    juce::TextButton clearButton { "CLEAR" };
    juce::TextButton laneClear[4];

    juce::Slider bpmSlider, brightSlider, crushSlider, spaceSlider, gainSlider;
    juce::Label  bpmLabel, brightLabel, crushLabel, spaceLabel, gainLabel;

    using SliderAttach = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttach> bpmAttach, brightAttach, crushAttach, spaceAttach, gainAttach;

    // which kit pad is currently loaded on each lane (for the lane label)
    int lanePad[4] { -1, -1, -1, -1 };

    void refreshPads();
    void refreshSteps();
    void onPadClicked (int padIndex);
    void onStepClicked (int lane, int step);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MCSynthAudioProcessorEditor)
};

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

//==============================================================================
//  Pad button
//==============================================================================
void MCSynthAudioProcessorEditor::PadButton::paintButton (juce::Graphics& g,
                                                          bool over, bool down)
{
    auto b = getLocalBounds().toFloat();

    auto fill = baseColour;
    if (down)      fill = fill.brighter (0.45f);
    else if (over) fill = fill.brighter (0.18f);

    g.setColour (fill);
    g.fillRect (b);

    // a bevelled edge, like a block in a hotbar
    g.setColour (juce::Colours::white.withAlpha (0.22f));
    g.drawLine (b.getX(), b.getY(), b.getRight(), b.getY(), 2.0f);
    g.drawLine (b.getX(), b.getY(), b.getX(), b.getBottom(), 2.0f);
    g.setColour (juce::Colours::black.withAlpha (0.38f));
    g.drawLine (b.getX(), b.getBottom(), b.getRight(), b.getBottom(), 2.0f);
    g.drawLine (b.getRight(), b.getY(), b.getRight(), b.getBottom(), 2.0f);
    g.drawRect (b, 1.0f);

    g.setColour (juce::Colours::black.withAlpha (0.72f));
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText (padName, getLocalBounds().reduced (2, 1),
                juce::Justification::centred, false);
}

//==============================================================================
//  Step button
//==============================================================================
void MCSynthAudioProcessorEditor::StepButton::paintButton (juce::Graphics& g,
                                                           bool over, bool down)
{
    auto col = isBeat ? juce::Colour (0xff6e6e6e) : juce::Colour (0xff5e5e5e);
    if (on) col = juce::Colour (0xffc8b400);
    if (down) col = col.brighter (0.3f);

    g.setColour (col);
    g.fillRect (getLocalBounds());

    g.setColour (juce::Colours::white.withAlpha (0.18f));
    g.drawLine (0.0f, 0.0f, (float) getWidth(), 0.0f, 1.0f);
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.drawRect (getLocalBounds(), 1);

    if (isCurrent)
    {
        g.setColour (juce::Colour (0xff4ad0ff));
        g.drawRect (getLocalBounds().reduced (1), 2);
    }
}

//==============================================================================
MCSynthAudioProcessorEditor::MCSynthAudioProcessorEditor (MCSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), proc (p)
{
    setSize (860, 620);

    auto& s = proc.apvts;

    // ---- kit selector ----
    addAndMakeVisible (kitBox);
    for (int i = 0; i < NUM_KITS; ++i) kitBox.addItem (KIT_NAMES[i], i + 1);
    kitBox.onChange = [this] { refreshPads(); };
    kitAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (s, "kit", kitBox);

    // ---- pad grid ----
    for (int i = 0; i < PADS_PER_KIT; ++i)
    {
        auto btn = std::make_unique<PadButton>();
        btn->index = i;
        btn->onClick = [this, i] { onPadClicked (i); };
        addAndMakeVisible (*btn);
        padButtons.push_back (std::move (btn));
    }

    // ---- step grid: 4 lanes x 16 steps ----
    for (int l = 0; l < StepSequencer::kLanes; ++l)
    {
        for (int st = 0; st < StepSequencer::kSteps; ++st)
        {
            auto b = std::make_unique<StepButton>();
            b->lane = l;
            b->step = st;
            b->isBeat = (st % 4 == 0);
            b->onClick = [this, l, st] { onStepClicked (l, st); };
            addAndMakeVisible (*b);
            stepButtons.push_back (std::move (b));
        }
        laneClear[l].setButtonText ("clear");
        laneClear[l].onClick = [this, l] { proc.sequencer().clearLane (l); lanePad[l] = -1; refreshSteps(); };
        addAndMakeVisible (laneClear[l]);
    }

    // ---- transport ----
    playButton.setClickingTogglesState (true);
    playButton.onClick = [this]
    {
        proc.sequencer().setPlaying (playButton.getToggleState());
    };
    addAndMakeVisible (playButton);

    clearButton.onClick = [this]
    {
        proc.sequencer().clear();
        for (int i = 0; i < 4; ++i) lanePad[i] = -1;
        refreshSteps();
    };
    addAndMakeVisible (clearButton);

    // ---- master knobs ----
    auto setupKnob = [this] (juce::Slider& sl, juce::Label& lb, const juce::String& text,
                             const juce::String& suffix)
    {
        addAndMakeVisible (sl);
        sl.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        sl.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 62, 15);
        sl.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff9cb04a));
        sl.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff3a3a22));
        sl.setColour (juce::Slider::thumbColourId, juce::Colour (0xffe8e0a8));
        sl.setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe8e0a8));
        sl.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        if (suffix.isNotEmpty()) sl.setTextValueSuffix (suffix);

        addAndMakeVisible (lb);
        lb.setText (text, juce::dontSendNotification);
        lb.setJustificationType (juce::Justification::centred);
        lb.setColour (juce::Label::textColourId, juce::Colour (0xffc8c090));
        lb.setFont (juce::Font (11.0f));
    };

    setupKnob (bpmSlider,     bpmLabel,     "TEMPO",     "");
    setupKnob (brightSlider,  brightLabel,  "BRIGHT",    "");
    setupKnob (crushSlider,   crushLabel,   "LO-FI",     "");
    setupKnob (spaceSlider,   spaceLabel,   "SPACE",     "");
    setupKnob (gainSlider,    gainLabel,    "GAIN",      "");

    bpmAttach    = std::make_unique<SliderAttach> (s, "bpm",    bpmSlider);
    brightAttach = std::make_unique<SliderAttach> (s, "bright", brightSlider);
    crushAttach  = std::make_unique<SliderAttach> (s, "crush",  crushSlider);
    spaceAttach  = std::make_unique<SliderAttach> (s, "space",  spaceSlider);
    gainAttach   = std::make_unique<SliderAttach> (s, "gain",   gainSlider);

    refreshPads();
    refreshSteps();

    startTimerHz (30);   // for the moving playhead
}

MCSynthAudioProcessorEditor::~MCSynthAudioProcessorEditor()
{
    stopTimer();
}

void MCSynthAudioProcessorEditor::refreshPads()
{
    const int k = proc.getKitIndex();
    if (k < 0 || k >= NUM_KITS) return;

    for (int i = 0; i < (int) padButtons.size(); ++i)
    {
        auto* b = padButtons[(size_t) i].get();
        b->padName = KITS[k][i].name;

        // colour by position, matching the web app's palette
        const float hue = 0.17f + (float) (i % 8) * 0.012f;
        b->baseColour = juce::Colour::fromHSV (hue, 0.40f,
                                               0.36f + (float) (i % 3) * 0.05f, 1.0f);
        b->repaint();
    }
    repaint();
}

void MCSynthAudioProcessorEditor::refreshSteps()
{
    for (auto& b : stepButtons)
    {
        b->on = proc.sequencer().getStep (b->lane, b->step) >= 0;
        const bool beyond = b->step >= proc.sequencer().getNumSteps();
        b->setEnabled (! beyond);
        b->repaint();
    }
}

void MCSynthAudioProcessorEditor::onPadClicked (int padIndex)
{
    // load this pad onto the first free lane, so the grid doubles as a selector
    int lane = -1;
    for (int i = 0; i < 4; ++i) if (lanePad[i] < 0) { lane = i; break; }
    if (lane < 0) lane = 0;                 // all busy: replace lane 0
    lanePad[lane] = padIndex;

    proc.setLastTriggeredPad (padIndex);
    proc.triggerPad (padIndex, 1.0f);        // audition it
}

void MCSynthAudioProcessorEditor::onStepClicked (int lane, int step)
{
    auto& seq = proc.sequencer();
    const bool currentlyOn = seq.getStep (lane, step) >= 0;

    if (currentlyOn)
    {
        seq.setStep (lane, step, -1);
    }
    else
    {
        // use the lane's loaded pad, else whatever pad was last touched
        const int pad = lanePad[lane] >= 0 ? lanePad[lane] : proc.getLastTriggeredPad();
        seq.setStep (lane, step, pad);
    }
    refreshSteps();
}

void MCSynthAudioProcessorEditor::timerCallback()
{
    const int cur = proc.uiStep.exchange (-1);
    if (cur < 0) return;

    for (auto& b : stepButtons)
    {
        const bool nowCur = (b->step == cur);
        if (nowCur != b->isCurrent)
        {
            b->isCurrent = nowCur;
            b->repaint();
        }
    }
}

//==============================================================================
void MCSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1e2014));

    g.setColour (juce::Colour (0xff2e3320));
    g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (6.0f), 6.0f);

    g.setColour (juce::Colour (0xff9cb04a));
    g.setFont (juce::Font (20.0f, juce::Font::bold));
    g.drawText ("MC SYNTH", 16, 8, 220, 24, juce::Justification::centredLeft);

    g.setColour (juce::Colour (0xff7a8a4a));
    g.setFont (juce::Font (11.0f));
    g.drawText ("120 sounds  ·  5 kits  ·  16-step machine",
                16, 30, 340, 16, juce::Justification::centredLeft);

    // section captions
    g.setColour (juce::Colour (0xff8d9a56));
    g.setFont (juce::Font (10.0f, juce::Font::bold));
    g.drawText ("PADS", 16, 74, 80, 12, juce::Justification::centredLeft);
    g.drawText ("MACHINE", 16, 320, 90, 12, juce::Justification::centredLeft);
}

void MCSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (14);
    area.removeFromTop (54);       // header

    // ---- kit row ----
    auto kitRow = area.removeFromTop (28);
    kitBox.setBounds (kitRow.removeFromLeft (200).reduced (0, 2));
    area.removeFromTop (10);

    // ---- pad grid ----
    auto grid = area.removeFromTop (200);
    const int pw = grid.getWidth() / kPadCols;
    const int ph = grid.getHeight() / kPadRows;
    for (int r = 0; r < kPadRows; ++r)
        for (int c = 0; c < kPadCols; ++c)
        {
            const int i = r * kPadCols + c;
            if (i < (int) padButtons.size())
                padButtons[(size_t) i]->setBounds (
                    grid.getX() + c * pw, grid.getY() + r * ph, pw - 3, ph - 3);
        }
    area.removeFromTop (14);

    // ---- transport ----
    auto transport = area.removeFromTop (26);
    playButton.setBounds (transport.removeFromLeft (90).reduced (0, 1));
    transport.removeFromLeft (6);
    clearButton.setBounds (transport.removeFromLeft (80).reduced (0, 1));
    area.removeFromTop (8);

    // ---- lanes ----
    auto lanes = area.removeFromTop (140);
    const int laneH = lanes.getHeight() / StepSequencer::kLanes;
    const int laneLabelW = 74;
    const int clearW = 46;

    for (int l = 0; l < StepSequencer::kLanes; ++l)
    {
        auto row = lanes.removeFromTop (laneH);
        auto clearArea = row.removeFromRight (clearW);
        laneClear[l].setBounds (clearArea.reduced (2, 6));

        juce::Label laneName;
        // (lane name is drawn in paint via the pad name; drawn below as text)
        juce::ignoreUnused (laneName);

        auto stepsArea = row.removeFromRight (row.getWidth() - laneLabelW);
        const int sw = stepsArea.getWidth() / StepSequencer::kNumSteps;
        for (int st = 0; st < StepSequencer::kNumSteps; ++st)
        {
            auto* b = stepButtons[(size_t) (l * StepSequencer::kNumSteps + st)].get();
            b->setBounds (stepsArea.getX() + st * sw, stepsArea.getY() + 3,
                          sw - 2, stepsArea.getHeight() - 6);
        }
    }
    area.removeFromTop (10);

    // ---- master knobs ----
    auto knobs = area;
    const int kw = knobs.getWidth() / 5;
    auto place = [this] (juce::Slider& sl, juce::Label& lb, juce::Rectangle<int> cell)
    {
        lb.setBounds (cell.removeFromTop (14));
        sl.setBounds (cell);
    };
    place (bpmSlider,    bpmLabel,    knobs.removeFromLeft (kw).reduced (4, 0));
    place (brightSlider, brightLabel, knobs.removeFromLeft (kw).reduced (4, 0));
    place (crushSlider,  crushLabel,  knobs.removeFromLeft (kw).reduced (4, 0));
    place (spaceSlider,  spaceLabel,  knobs.removeFromLeft (kw).reduced (4, 0));
    place (gainSlider,   gainLabel,   knobs.reduced (4, 0));
}
