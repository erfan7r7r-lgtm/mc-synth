#include "PluginEditor.h"

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
