/*
  ==============================================================================

    InputModule.cpp
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#include "InputModule.h"


InputModule::InputModule() 
{
    //
    addAndMakeVisible ( InputSlider );  

    InputSlider.setRange ( 0, 100 );
    InputSlider.setSliderStyle ( Slider::LinearVertical );
    InputSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20);   
}


InputModule::~InputModule() 
{
    //
}


void InputModule::paint (Graphics& g)
{
    g.setColour (BORDER_COLOR);

    buildModuleBorder ( g, CORNERSIZE, THICKNESS, OFFSET );
    
    g.drawText ("INPUT", 0, 25, getWidth(), getHeight(), Justification::centredTop); 

}



void InputModule::resized ()
{
    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows    = { Track (1_fr), Track (10_fr) };
    grid.templateColumns = { Track (1_fr) };

    grid.items = {
        GridItem ( nullptr ),
        GridItem ( InputSlider )
    };

    Rectangle <int> bounds = getLocalBounds();

    bounds = moduleInternalsBounds ( bounds,
                                     MODULE_INSIDE_OFFSET,
                                     OFFSET,
                                     THICKNESS);

    grid.performLayout ( bounds );

}
