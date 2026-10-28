cmake_minimum_required(VERSION 3.22)

project(MCSynth VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
#  JUCE is downloaded automatically the first time you configure.
#  (You need Git installed for this. See README.md.)
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(JUCE)

# ---------------------------------------------------------------------------
#  The plugin.  VST3 = the format FL Studio loads.
#  Standalone = a .exe you can run on its own to test the sounds.
# ---------------------------------------------------------------------------
juce_add_plugin(MCSynth
    COMPANY_NAME              "MC"
    IS_SYNTH                  TRUE
    NEEDS_MIDI_INPUT          TRUE
    NEEDS_MIDI_OUTPUT         FALSE
    IS_MIDI_EFFECT            FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD   TRUE
    PLUGIN_MANUFACTURER_CODE  Mcsy
    PLUGIN_CODE               Mcsy
    FORMATS                   VST3 Standalone
    PRODUCT_NAME              "MC Synth"
)

target_sources(MCSynth PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

# PadTable.h is a header only; make sure it is visible where the code includes it
target_include_directories(MCSynth PRIVATE Source)

target_compile_definitions(MCSynth PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(MCSynth PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
