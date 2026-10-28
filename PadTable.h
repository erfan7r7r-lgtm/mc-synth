#pragma once
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
