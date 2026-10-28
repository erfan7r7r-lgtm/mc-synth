#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

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
