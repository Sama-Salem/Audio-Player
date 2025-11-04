#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    //Background:
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour::fromRGB(0,0,0));
    //Button:
    setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 20,147));    
    setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(255,255 ,255));
    setColour(juce::TextButton::textColourOnId, juce::Colour::fromRGB(0, 255, 0));

    //Slider:
    setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 20, 147)); 
    setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(120, 120, 120));
    //Label
    setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 255 ,255));
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    juce::Colour baseColour = backgroundColour;
    if (shouldDrawButtonAsDown)
        baseColour = juce::Colour::fromRGB(0, 200, 0); 
    else if (shouldDrawButtonAsHighlighted)
        baseColour = juce::Colour::fromRGB(255, 50, 50); 
    else
        baseColour = backgroundColour;

    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds.reduced(3.0f), 10.0f);
}

void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool, bool)
{
    g.setColour(button.findColour(juce::TextButton::textColourOffId));
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float, float, const juce::Slider::SliderStyle, juce::Slider& slider)
{
    juce::Colour track = slider.findColour(juce::Slider::trackColourId);
    juce::Colour thumb = slider.findColour(juce::Slider::thumbColourId);

    g.setColour(track);
    g.fillRoundedRectangle(juce::Rectangle<float>(x, y + height / 2 - 3, width, 6), 3.0f);

    g.setColour(thumb);
    g.fillEllipse(sliderPos - 8, y + height / 2 - 8, 16, 16);
}
