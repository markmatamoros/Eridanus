/*
  ==============================================================================

    LFOModule.cpp
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#include "LFOModule.h"

LfoModule::LfoModule ( String title ) : lfoTitle (title)
{
    addAndMakeVisible ( RateSlider );
    RateSlider.setRange ( 0, 10.0 );
    RateSlider.setSliderStyle ( Slider::LinearVertical );
    RateSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
    RateSlider.setValue( 1.0 );

    addAndMakeVisible( RateLabel );
    RateLabel.setText( "Rate", dontSendNotification );
    RateLabel.setJustificationType( Justification::centred );
    
    addAndMakeVisible ( DepthSlider );
    DepthSlider.setRange ( 0, 1.0 );
    DepthSlider.setSliderStyle ( Slider::LinearVertical );
    DepthSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
    DepthSlider.setValue( 0.0 );

    addAndMakeVisible( DepthLabel );
    DepthLabel.setText( "Depth", dontSendNotification );
    DepthLabel.setJustificationType( Justification::centred );
    
    // TITLE
    addAndMakeVisible( TitleLabel );
    TitleLabel.setText( lfoTitle, dontSendNotification );
    TitleLabel.setJustificationType( Justification::centred );
}    
    
void LfoModule::paint (Graphics& g)
{
    g.setColour (BORDER_COLOR);

    CustomComponent::buildModuleBorder ( g, CORNERSIZE, THICKNESS, OFFSET );
}

void LfoModule::resized ()
{
    // 
    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows = { 
        Track (1_fr)
    };
    grid.templateColumns = { 
        Track (1_fr) 
    };
  
    grid.autoRows = Track (1_fr);
    grid.autoColumns = Track (1_fr);

    grid.items = {
        GridItem ( TitleLabel  ).withArea( 1 , 1, 1 , 3 ),
        GridItem ( RateSlider  ).withArea( 2 , 1, 10, 2 ),
        GridItem ( DepthSlider ).withArea( 2 , 2, 10, 3 ),
        GridItem ( RateLabel   ).withArea( 10, 1, 10, 2 ),
        GridItem ( DepthLabel  ).withArea( 10, 2, 10, 3 )
    };

    Rectangle <int> bounds = getLocalBounds();
    bounds = moduleInternalsBounds ( bounds, 
                                     MODULE_INSIDE_OFFSET, 
                                     OFFSET, 
                                     THICKNESS);

    grid.performLayout ( bounds );
}


bool LfoModule::isRateSlider ( Slider* slider )
{
    return slider == &RateSlider;
}

bool LfoModule::isDepthSlider( Slider* slider )
{
    return slider == &DepthSlider;
}


float LfoModule::getRateSliderValue ()
{
    return RateSlider.getValue();
}

float LfoModule::getDepthSliderValue ()
{
    return DepthSlider.getValue();
}

void LfoModule::setRateSliderValue ( float v )
{
    RateSlider.setValue( v );
}

void LfoModule::setDepthSliderValue ( float v )
{
    DepthSlider.setValue( v );
}

