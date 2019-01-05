/*
  ==============================================================================

    CustomComponent.h
    Created: 12 Oct 2018 3:43:03pm
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define CORNERSIZE   10.0f
#define THICKNESS    2.5f
#define OFFSET       5.0F
#define BORDER_COLOR Colours::white

#define MODULE_INSIDE_OFFSET 1.75

using namespace std;

//==============================================================================
/*
*/
class CustomComponent    : public Component
{
public:
    CustomComponent();
    ~CustomComponent();

    void paint (Graphics&) override;
    void resized() override;

    void buildModuleBorder(Graphics& g, float cornerSize, float thickness, float offset);
    Rectangle <int> moduleInternalsBounds (Rectangle <int> bounds, 
                                           float inside_offset,
                                           float offset,
                                           float thickness);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomComponent)
};
