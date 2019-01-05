/*
  ==============================================================================

    LFOModule.h
    Created: 26 Oct 2018 8:29:19am
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"

// LFO
class LfoModule : public CustomComponent
{
public:

    LfoModule ( String type ); 
    ~LfoModule () {};
    void paint (Graphics& g) override;    
    void resized () override;

    bool isRateSlider(Slider* slider);
    bool isDepthSlider(Slider* slider);
    
    // GET
    float getDepthSliderValue();
    float getRateSliderValue();

    // SET
    void setDepthSliderValue( float v );
    void setRateSliderValue( float v );

   

private:
    Slider RateSlider;
    Slider DepthSlider;
    
    String lfoTitle;
    
    Label RateLabel;
    Label DepthLabel;
    Label TitleLabel;
};
 
