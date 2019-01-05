/*
  ==============================================================================

    SaturationModule.h
    Created: 26 Oct 2018 8:35:48am
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"

// SATURATION
class SaturationModule : public CustomComponent
{
public:
    SaturationModule ();                       
    ~SaturationModule();
                  
    void paint (Graphics& g) override;
    void resized () override;
    
    void buttonUpdate( String text );
    
    int getSaturationType();
    
    TextButton& getTapeButton();
    TextButton& getTubeButton();
    
    bool isDriveSlider(Slider* slider);
    
    float getDriveSliderValue();

    void setDriveSliderValue( float v );
    
private:    
    Slider DriveKnob;
    Label  DriveLabel;
    
    int saturationType = 1;
    
    TextButton TapeButton;
    TextButton TubeButton;
    
	Label TitleLabel;
	
	String Title;
};
