/*
  ==============================================================================

    OutputModule.h
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"
   
// OUTPUT
class OutputModule : public CustomComponent
{
public:
    OutputModule ();
    ~OutputModule ();

    void paint (Graphics& g) override;   
    void resized () override;
    
    bool isOutputSlider(Slider* slider);
    
    float getOutputSliderValue();

    void setOutputSliderValue( float v );

private:
    Slider OutputSlider;
    Label OutputSliderLabel;

    Slider OutputFeedbackSlider;
    Label OutputFeedbackLabel;
    
	Label TitleLabel;	
	String Title;
};
