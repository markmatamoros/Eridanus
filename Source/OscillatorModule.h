/*
  ==============================================================================

    OscillatorModule.h
    Created: 26 Oct 2018 7:59:28am
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"


enum LengthState 
{
    FOUR,
    EIGHT,
    SIXTEEN
};

enum WaveState 
{
    SIN,
    SAW,
    TRIANGLE,
    SQUARE
};


class OscillatorModule : public CustomComponent
{
public:
	OscillatorModule ( String title );
	~OscillatorModule ();

    void paint (Graphics& g) override;
    void resized () override;

    void lengthButtonClicked ( Button* current );
    void waveButtonClicked ( Button* current );
    
    void changeWaveState (WaveState newWave);
    void changeLengthState (LengthState newLength);    

    bool isLevelSlider( Slider* slider );
    bool isFreqSlider( Slider* slider );
    bool isFineTuneSlider( Slider* slider );
    bool isLength4Button( Button *button);
    bool isLength8Button( Button *button);
    bool isLength16Button( Button *button);
    bool isSinWaveButton( Button *button );
    bool isSawWaveButton( Button *button );
    bool isTriangleWaveButton( Button *button );
    bool isSquareWaveButton( Button *button );
    bool isThis ( Button *button );

    float getLevelSliderValue();
    float getFreqSliderValue();
    float getFineTuneSliderValue();
    float getOscMult();

    TextButton& getLength4Button();
    TextButton& getLength8Button();
    TextButton& getLength16Button();

    TextButton& getSinButton();
    TextButton& getSawButton();
    TextButton& getTriangleButton();
    TextButton& getSquareButton();

    String getOscType();

    void setLevelSliderValue( float v );
    void setFreqSliderValue( float v );
    void setFineTuneSliderValue( float v );
    void setOscMult( float v );
    
    
private:
    Slider levelSlider;
    Slider freqSlider;
    Slider fineTuneSlider;

    Label levelLabel;
    Label freqLabel;
    Label fineTune;
    Label oscTypeLabel;
    Label oscLength;
    Label oscLabel;

    TextButton length4;
    TextButton length8;
    TextButton length16;
    LengthState lenState;

    TextButton sinButton;
    TextButton sawButton;
    TextButton triButton;
    TextButton sqrButton;
    WaveState waveState;
    
    float oscMult;

    String oscType;
    String oscTitle;
};
