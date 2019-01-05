/*
  ==============================================================================

    OutputModule.h
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"


class InputModule : public CustomComponent
{
public:
    InputModule();
    ~InputModule();

    void paint (Graphics& g) override;
    void resized () override;


private:
    Slider InputSlider;
    Label  InputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputModule)
};
