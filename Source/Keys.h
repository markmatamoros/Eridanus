/*
  ==============================================================================

    Keys.h
    Created: 2 Nov 2018 11:19:38am
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"

class Keys : public CustomComponent// ,
                    // Button::Listener
{
public:
    Keys();
    ~Keys();

    void paint( Graphics& g ) override;
    void resized () override;

private:
    ToggleButton key;
};
