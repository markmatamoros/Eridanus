/*
  ==============================================================================

    SaturationModule.cpp
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#include "SaturationModule.h"

SaturationModule::SaturationModule ( ) : Title ("SATURATION")
{
    //
    addAndMakeVisible ( DriveKnob );
    DriveKnob.setRange ( 0.0f, 1.0f );
    DriveKnob.setSliderStyle ( Slider::RotaryVerticalDrag );
    DriveKnob.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );

    addAndMakeVisible( DriveLabel );
    DriveLabel.setText( "Drive", dontSendNotification );
    DriveLabel.setJustificationType( Justification::centred );
    
    addAndMakeVisible ( TapeButton );
    TapeButton.setButtonText ( "TP" );

    addAndMakeVisible ( TubeButton );
    TubeButton.setButtonText ( "TB" );
    
    // TITLE
    addAndMakeVisible( TitleLabel );
    TitleLabel.setText( Title, dontSendNotification );
    TitleLabel.setJustificationType( Justification::centred );
}

SaturationModule::~SaturationModule ( )
{
}
              
void SaturationModule::paint (Graphics& g)
{
    g.setColour (BORDER_COLOR);
    buildModuleBorder ( g, CORNERSIZE, THICKNESS, OFFSET );
}

void SaturationModule::resized ()
{
    // 
    Grid grid;

    using Track = Grid::TrackInfo;
    using Px    = Grid::Px;
    
    grid.templateRows = { 
        Track (1_fr)
    };
    grid.templateColumns = { 
        Track (1_fr) 
    };
    
    grid.setGap( Px ( 3_px ) );
  
    grid.autoRows = Track (1_fr);
    grid.autoColumns = Track (1_fr);

    
    grid.templateColumns = { Track (1_fr) };

    grid.items = {
        GridItem ( TitleLabel ).withArea( 1 , 1, 1 , 10 ),
        GridItem ( TapeButton ).withArea( 2 , 3, 3 , 8 ),
        GridItem ( TubeButton ).withArea( 3 , 3, 4 , 8 ),

        GridItem ( DriveLabel ).withArea( 5 , 2, 5 , 9 ),
        GridItem ( DriveKnob  ).withArea( 5 , 2, 8 , 9 )
    };

    Rectangle <int> bounds = getLocalBounds();

    bounds = moduleInternalsBounds ( bounds,
                                     MODULE_INSIDE_OFFSET,
                                     OFFSET,
                                     THICKNESS);


    grid.performLayout ( bounds );
}

void SaturationModule::buttonUpdate( String text )
{    
    if (text == "TP")
        saturationType = 1;

    else if (text  == "TB")
        saturationType = 2;
}

int SaturationModule::getSaturationType()
{    
    return saturationType;
}

TextButton& SaturationModule::getTapeButton()
{
    return TapeButton;
}

TextButton& SaturationModule::getTubeButton()
{
    return TubeButton;
}

bool SaturationModule::isDriveSlider ( Slider* slider )
{
    return slider == &DriveKnob;
}

float SaturationModule::getDriveSliderValue ()
{
    return DriveKnob.getValue();
}

void SaturationModule::setDriveSliderValue ( float v )
{
    DriveKnob.setValue( v );
}
