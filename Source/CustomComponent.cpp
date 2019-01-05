/*
  ==============================================================================

    CustomComponent.cpp
    Created: 12 Oct 2018 3:43:03pm
    Author:  allen

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomComponent.h"

//==============================================================================
CustomComponent::CustomComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

}

void CustomComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void CustomComponent::buildModuleBorder(Graphics& g, float cornerSize, float thickness, float offset)
{
    float x1 = offset + thickness,
          y1 = offset + thickness,
          x2 = getWidth()  - (offset + thickness) * 1.0f,
          y2 = getHeight() - (offset + thickness) * 1.0f;

    Point <float> p1 = { x1, y1 };
    Point <float> p2 = { x2, y2 };

    Rectangle <float> border { p1, p2 };

    g.drawRoundedRectangle (border, cornerSize, thickness);
}


Rectangle <int> CustomComponent::moduleInternalsBounds (Rectangle <int> bounds,
                                       float inside_offset,
                                       float offset,
                                       float thickness)
{
    float change = ( thickness + offset ) * inside_offset;

    bounds.setX ( bounds.getX() + change );
    bounds.setY ( bounds.getY() + change );
    bounds.setWidth  ( bounds.getWidth() - ( change * 2.0 ) );
    bounds.setHeight ( bounds.getHeight() - ( change * 2.0 ) - 10 );

    return bounds;
}


