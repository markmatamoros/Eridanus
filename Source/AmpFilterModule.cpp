/*
  ==============================================================================

    AmpFilterModule.cpp
    Created: 12 Oct 2018 4:19:38pm
    Author:  allen

  ==============================================================================
*/

#include "AmpFilterModule.h"


AmpFilterModule::AmpFilterModule ( ) : Title ( "AMP ENVELOPE" )
{
	addAndMakeVisible ( AttackSlider );
	AttackSlider.setRange ( 0.0f, 5.0f );
	AttackSlider.setSliderStyle ( Slider::LinearVertical );
	AttackSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
	AttackSlider.setValue(2.0);

    addAndMakeVisible ( DecaySlider );
	DecaySlider.setRange ( 0.0f, 3.0f );
	DecaySlider.setSliderStyle ( Slider::LinearVertical );
	DecaySlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
	DecaySlider.setValue(0.0);
	
	addAndMakeVisible ( SustainSlider );
	SustainSlider.setRange ( 0.0f, 1.0f );
	SustainSlider.setSliderStyle ( Slider::LinearVertical );
	SustainSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
    SustainSlider.setValue( 1.0f );

	addAndMakeVisible ( ReleaseSlider );
	ReleaseSlider.setRange ( 0.0f, 5.0f );
	ReleaseSlider.setSliderStyle ( Slider::LinearVertical );
	ReleaseSlider.setTextBoxStyle ( Slider::TextBoxBelow, true, 50, 20 );
    ReleaseSlider.setValue(2.0);
	
	addAndMakeVisible ( AttackSliderLabel );
    AttackSliderLabel.setText("A", dontSendNotification);
    AttackSliderLabel.setJustificationType ( Justification::centred );

    addAndMakeVisible ( DecaySliderLabel );
    DecaySliderLabel.setText("D", dontSendNotification);
    DecaySliderLabel.setJustificationType ( Justification::centred );

    addAndMakeVisible ( SustainSliderLabel );
    SustainSliderLabel.setText("S", dontSendNotification);
    SustainSliderLabel.setJustificationType ( Justification::centred );
            
    addAndMakeVisible ( ReleaseSliderLabel );
    ReleaseSliderLabel.setText("R", dontSendNotification);
    ReleaseSliderLabel.setJustificationType ( Justification::centred );
    
    // TITLE
    addAndMakeVisible( TitleLabel );
    TitleLabel.setText( Title, dontSendNotification );
    TitleLabel.setJustificationType( Justification::centred );

}

AmpFilterModule::~AmpFilterModule ()
{
}

void AmpFilterModule::paint (Graphics& g) 
{
	g.setColour (BORDER_COLOR);
	buildModuleBorder (g, CORNERSIZE, THICKNESS, OFFSET);
}

void AmpFilterModule::resized () 
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
	    GridItem ( TitleLabel         ).withArea( 1 , 1, 1, 5 ),
	    
	    GridItem ( AttackSlider       ).withArea( 2 , 1, 10, 2 ),
	    GridItem ( DecaySlider        ).withArea( 2 , 2, 10, 3 ),
	    GridItem ( SustainSlider      ).withArea( 2 , 3, 10, 4 ),
	    GridItem ( ReleaseSlider      ).withArea( 2 , 4, 10, 5 ),
	    
	    GridItem ( AttackSliderLabel  ).withArea( 10 , 1, 10, 2 ),
        GridItem ( DecaySliderLabel   ).withArea( 10 , 2, 10, 3 ),
        GridItem ( SustainSliderLabel ).withArea( 10 , 3, 10, 4 ),
        GridItem ( ReleaseSliderLabel ).withArea( 10 , 4, 10, 5 )
	};

	Rectangle <int> bounds = getLocalBounds();
	bounds = moduleInternalsBounds ( bounds,
	                                 MODULE_INSIDE_OFFSET,
	                                 OFFSET,
	                                 THICKNESS);

	grid.performLayout ( bounds );
}

float AmpFilterModule::getAttackSliderValue ()
{
    return AttackSlider.getValue();
}

float AmpFilterModule::getDecaySliderValue ()
{
    return DecaySlider.getValue();
}

float AmpFilterModule::getSustainSliderValue ()
{
    return SustainSlider.getValue();
}

float AmpFilterModule::getReleaseSliderValue ()
{
    return ReleaseSlider.getValue();
}











