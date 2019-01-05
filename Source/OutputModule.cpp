/*
  ==============================================================================

    OutputModule.cpp
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#include "OutputModule.h"

OutputModule::OutputModule () : Title ("OUTPUT")
{
    
    addAndMakeVisible ( OutputSlider );
    OutputSlider.setRange ( 0.0, 1.0, 0 );
    OutputSlider.setSliderStyle (Slider::LinearVertical);
    OutputSlider.setTextBoxStyle (Slider::TextBoxBelow, true, 50, 20);
    OutputSlider.setValue( 1.0f );
    
    // TITLE
    addAndMakeVisible( TitleLabel );
    TitleLabel.setText( Title, dontSendNotification );
    TitleLabel.setJustificationType( Justification::centred );
}

OutputModule::~OutputModule ()
{}

void OutputModule::paint (Graphics& g)
{
    g.setColour (BORDER_COLOR);

    CustomComponent::buildModuleBorder( g, CORNERSIZE, THICKNESS, OFFSET );
}

void OutputModule::resized ()
{

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
        GridItem ( TitleLabel   ).withArea( 1, 1, 1 , 2 ),
        GridItem ( OutputSlider ).withArea( 2, 1, 10, 2 )
    };

    Rectangle<int> bounds = getLocalBounds();

    bounds = moduleInternalsBounds ( bounds, MODULE_INSIDE_OFFSET, OFFSET, THICKNESS );

    grid.performLayout ( bounds );
}

bool OutputModule::isOutputSlider ( Slider* slider )
{
    return slider == &OutputSlider;
}

float OutputModule::getOutputSliderValue ()
{
    return OutputSlider.getValue();
}

void OutputModule::setOutputSliderValue ( float v )
{
    OutputSlider.setValue( v );
}

