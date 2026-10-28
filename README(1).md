# MC Synth — a VST3 for FL Studio

**120 sounds · 5 kits · a 16-step machine.** The same instrument as the MC Pads web
app, rebuilt as a plugin so it runs inside FL Studio.

## What's inside

| Kit | Sounds |
|---|---|
| **BLOCK KIT** | grass, dirt, stone, wood, sand, gravel, glass, wool, snow, leaves, nether, obsidian, portal, anvil, chest, door, lever, button, fire, water, lava, fuse, XP, ding |
| **BLIP KIT** | select, click, confirm, back, error, pop, blips, jump, hurt, oof, death, level up, achievement, villager, zombie, creeper, skeleton, enderman, blaze, ghast, ender, totem |
| **MELODY KIT** | two full octaves of notes plus low bass, bell, chime and pad |
| **MUSIC KIT** | piano, harp, bass, sub, pluck, kick, snare, hat, tom, clap, shaker, cowbell, organ, strings, choir |
| **EPIC KIT** | cellos, bass drone, sub boom, six theme notes, four horns, two choirs, war drum, timpani, deep boom, riser, tension, bell toll |

## Build it (Windows + FL Studio)

### 1. Install the tools (once)

| Tool | Where | During install |
|---|---|---|
| Visual Studio 2022 | visualstudio.microsoft.com | tick **Desktop development with C++** |
| CMake | cmake.org/download | tick **Add CMake to the system PATH** |
| Git | git-scm.com | needed so CMake can download JUCE |

Restart your PC afterwards so the PATH changes take effect.

### 2. Open a terminal in this folder

In File Explorer, open the `mc-synth` folder, click the address bar, type `cmd`, press Enter.

### 3. Configure and build

```bat
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The first command downloads JUCE (a few minutes). The build takes 5–15 minutes the
first time. Later builds are fast.

### 4. Where the plugin lands

`COPY_PLUGIN_AFTER_BUILD` is on, so the finished file is copied to:

```
C:\Program Files\Common Files\VST3\MC Synth.vst3
```

A standalone `.exe` also appears in `build\MCSynth_artefacts\Release\Standalone\` —
run it to hear the sounds without opening FL Studio.

### 5. Load it in FL Studio

1. **Options → Manage plugins → Find more plugins**
2. Make sure `C:\Program Files\Common Files\VST3` is in the search paths, then **Start scan**
3. Channel Rack → click **+** → search **MC Synth**

## How to use it

**The pad grid (24 pads)** — click any pad to hear it. The pad loads onto the next
free lane, ready for the machine.

**The machine (4 lanes × 16 steps)** — click cells to place the lane's pad on that
step. Hit **PLAY** to loop. **clear** on a row empties that row; **CLEAR** empties
everything. Tempo is the TEMPO knob, 60–190 BPM.

**Playing it as an instrument** — send MIDI notes from a piano roll and MC Synth
plays a clean pitched tone (square wave) that follows the keyboard. Good for bass
lines and melodies. The pads are for the sound effects themselves.

**The master knobs**

| Knob | What it does |
|---|---|
| **TEMPO** | speed of the machine |
| **BRIGHT** | filter brightness. Deep pads stay dark even at the top, so cellos keep their body |
| **LO-FI** | bitcrusher. At 0 it is fully bypassed, so the sound is clean |
| **SPACE** | reverb send |
| **GAIN** | output level |

Your pattern and lane assignments are saved with the FL project, so they come back
when you reopen it.

## Files

```
mc-synth/
├── CMakeLists.txt            build setup
├── README.md                 this file
└── Source/
    ├── PadTable.h            all 120 sounds, generated from the web app
    ├── PluginProcessor.h     voice, sequencer and processor declarations
    ├── PluginProcessor.cpp   oscillators, filters, envelopes, crusher, timing
    ├── PluginEditor.h        UI declarations
    └── PluginEditor.cpp      pad grid, step grid, transport, knobs
```

## If the build fails

- **"cmake is not recognised"** — reinstall CMake and tick the PATH box.
- **"Git not found" during configure** — install Git, or download JUCE 7.0.12
  yourself and edit the `FetchContent_Declare` block in `CMakeLists.txt` to point
  at that folder.
- **FL Studio does not list it** — check that `MC Synth.vst3` exists in the VST3
  folder above and that this folder is in FL's search paths.
- **No sound from the pads** — make sure the plugin is on a channel that receives
  input, and that GAIN is not at zero.

## Note on the sounds

Every sound here is synthesised live from oscillators — there are no sample files
in this project. That means it builds small, loads instantly, and the sounds are
original rather than borrowed from any existing game or film.

## Licence

JUCE is used under its own terms (GPL or commercial — see juce.com/licensing).
The code in this folder is yours to use, change and rebuild freely.
