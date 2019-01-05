/*
  ==============================================================================

    OscillatorModule.cpp
    Created: 25 Oct 2018 7:09:22am
    Author:  allen

  ==============================================================================
*/

#include "OscillatorModule.h"

OscillatorModule::OscillatorModule ( String title ) 
    : oscTitle (title)
{
    // LEVEL
    addAndMakeVisible ( levelSlider );
    levelSlider.setRange ( 0.0f, 0.3f );
    levelSlider.setTextBoxStyle ( Slider::TextBoxRight, false, 50, 20 );
    levelSlider.setValue(0.3);
    levelSlider.setSliderStyle ( Slider::LinearBarVertical );
            
    addAndMakeVisible(levelLabel);
    levelLabel.setText("Amp", dontSendNotification);   
    levelLabel.setJustificationType( Justification::centred );
    
    // FINE TUNE
    addAndMakeVisible(fineTuneSlider);
    fineTuneSlider.setRange(-1.0, 1.0);
    fineTuneSlider.setTextBoxStyle(Slider::TextBoxRight, false, 50, 20);
    fineTuneSlider.setValue(0.0);
    fineTuneSlider.setSliderStyle ( Slider::LinearBar );
    
    addAndMakeVisible(fineTune);
    fineTune.setText("Tune", dontSendNotification);
    fineTune.setJustificationType( Justification::centred );

    // LENGTH
    addAndMakeVisible( length4 );
    length4.setButtonText( "16'" );
   
    addAndMakeVisible( length8 );
    length8.setButtonText( "8'" );
   
    addAndMakeVisible( length16 );
    length16.setButtonText( "4'" );

    // HACK TO FORCE EIGHT
    changeLengthState( FOUR  );
    changeLengthState( EIGHT );

    // WAVE FORM
    addAndMakeVisible( sinButton );
    sinButton.setButtonText( "Sin" );
 
    addAndMakeVisible( sawButton );
    sawButton.setButtonText( "Saw" );
      
    addAndMakeVisible( triButton );
    triButton.setButtonText( "Tri" );
    
    addAndMakeVisible( sqrButton );
    sqrButton.setButtonText( "Sqr" );

    // HACK TO FOR SIN WAVE
    changeWaveState( SAW );
    changeWaveState( SIN );

    // TITLE
    addAndMakeVisible( oscLabel );
    oscLabel.setText( oscTitle, dontSendNotification );
    oscLabel.setJustificationType( Justification::centred );
}


OscillatorModule::~OscillatorModule() {}


void OscillatorModule::paint (Graphics& g)
{
    g.setColour ( BORDER_COLOR );
    buildModuleBorder ( g, CORNERSIZE, THICKNESS, OFFSET );
}


void OscillatorModule::resized ()
{

    Grid grid;
    using Track = Grid::TrackInfo;
    using Px    = Grid::Px;

    grid.templateRows = { 
        Track (1_fr)
    };
    grid.templateColumns = { 
        Track (1_fr) 
    };
  
    grid.autoRows = Track (1_fr);
    grid.autoColumns = Track (1_fr);
    
    grid.setGap( Px ( 3_px ) );

    grid.items = {
        GridItem ( oscLabel ).withArea( 1, 1, 3, 7 ),

        GridItem ( length4  ).withArea( 3, 1, 5, 1 ),
        GridItem ( length8  ).withArea( 5, 1, 7, 1 ),
        GridItem ( length16 ).withArea( 7, 1, 9, 1 ),

        GridItem ( fineTuneSlider ).withArea( 7, 2, 9, 6 ),
        GridItem ( fineTune       ).withArea( 6, 2, 6, 6 ),
        
        GridItem ( levelSlider    ).withArea( 3, 6, 9, 6 ),
        GridItem ( levelLabel     ).withArea( 2, 6, 2, 6 ),
        
        GridItem ( sinButton ).withArea( 3, 2 , 5, 3 ),
        GridItem ( sawButton ).withArea( 3, 3 , 5, 4 ),
        GridItem ( triButton ).withArea( 3, 4 , 5, 5 ),
        GridItem ( sqrButton ).withArea( 3, 5 , 5, 6 ),
    };

    Rectangle <int> bounds = getLocalBounds();
    bounds = moduleInternalsBounds( bounds, MODULE_INSIDE_OFFSET, OFFSET, THICKNESS );

    grid.performLayout ( bounds );

}

void OscillatorModule::lengthButtonClicked( Button* current )
{
    if ( current == &length4 )
        changeLengthState( FOUR );

    else if (current == &length8 )
        changeLengthState( EIGHT );

    else if ( current == &length16 )
        changeLengthState( SIXTEEN );
}


void OscillatorModule::waveButtonClicked( Button* current )
{
    if (current == &sinButton) 
        changeWaveState( SIN );

    else if (current == &sawButton)
        changeWaveState( SAW );

    else if (current == &triButton)
        changeWaveState( TRIANGLE );

    else if (current == &sqrButton)
        changeWaveState( SQUARE );

    else
        cout << "waveButtonClicked ERROR" << endl;
}


void OscillatorModule::changeWaveState ( WaveState newWave )
{
    if (waveState != newWave) {
        waveState = newWave;

        switch (waveState)
        {
            case SIN:
                oscType = "Sin";
                sinButton.setEnabled( false );
                sawButton.setEnabled( true  );
                triButton.setEnabled( true  );
                sqrButton.setEnabled( true  );
                break;

            case SAW:
                oscType = "Saw";
                sinButton.setEnabled( true  );
                sawButton.setEnabled( false );
                triButton.setEnabled( true  );
                sqrButton.setEnabled( true  );
                break;


            case TRIANGLE:
                oscType = "Tri";
                sinButton.setEnabled( true  );
                sawButton.setEnabled( true  );
                triButton.setEnabled( false );
                sqrButton.setEnabled( true  );
                break;


            case SQUARE:
                oscType = "Sqr";
                sinButton.setEnabled( true  );
                sawButton.setEnabled( true  );
                triButton.setEnabled( true  );
                sqrButton.setEnabled( false );
                break;

            default: 
                cout << "changeWaveState ERROR" << endl;
       }
    }
}


void OscillatorModule::changeLengthState ( LengthState newLength )
{

    if (lenState != newLength) {
        lenState = newLength;

        switch (lenState)
        {

            case FOUR:  
                oscMult = 0.5f;
                length4 .setEnabled( false );
                length8 .setEnabled( true );
                length16.setEnabled( true );
                break;

            case EIGHT:  
                oscMult = 1.0f;
                length4 .setEnabled( true );
                length8 .setEnabled( false );
                length16.setEnabled( true );
                break;

            case SIXTEEN:  
                oscMult = 2.0f;
                length4 .setEnabled( true );
                length8 .setEnabled( true );
                length16.setEnabled( false );
                break;
    
        }
    }
}


bool OscillatorModule::isLevelSlider( Slider* slider )
{
    return slider == &levelSlider;
}


bool OscillatorModule::isFreqSlider( Slider* slider )
{
    return slider == &freqSlider;
}


bool OscillatorModule::isFineTuneSlider( Slider* slider )
{
    return slider == &fineTuneSlider;
}


bool OscillatorModule::isLength4Button( Button *button ) 
{
    return button == &length4;
}


bool OscillatorModule::isLength8Button( Button *button ) 
{
    return button == &length8;
}


bool OscillatorModule::isLength16Button( Button *button )
{
    return button == &length16;
}


bool OscillatorModule::isSinWaveButton( Button *button )
{
    return button == &sinButton;
}


bool OscillatorModule::isSawWaveButton( Button *button )
{
    return button == &sawButton;
}


bool OscillatorModule::isTriangleWaveButton( Button *button )
{
    return button == &triButton;
}


bool OscillatorModule::isSquareWaveButton( Button *button )
{
    return button == &sqrButton;
}


bool OscillatorModule::isThis( Button *button ) 
{
    return isLength4Button( button )
        || isLength8Button( button )
        || isLength16Button( button )
        || isSinWaveButton( button )
        || isSawWaveButton( button )
        || isTriangleWaveButton( button )
        || isSquareWaveButton( button );
}


float OscillatorModule::getLevelSliderValue()
{
    return levelSlider.getValue();
}


void OscillatorModule::setLevelSliderValue( float v )
{
    levelSlider.setValue( v );
}



float OscillatorModule::getFreqSliderValue()
{
    return freqSlider.getValue();
}


void OscillatorModule::setFreqSliderValue( float v )
{
    freqSlider.setValue( v );
}


float OscillatorModule::getFineTuneSliderValue()
{
    return fineTuneSlider.getValue();
}


void OscillatorModule::setFineTuneSliderValue( float v )
{
    fineTuneSlider.setValue( v );
}


TextButton& OscillatorModule::getLength4Button()
{
    return length4;
}


TextButton& OscillatorModule::getLength8Button()
{
    return length8;
}


TextButton& OscillatorModule::getLength16Button()
{
    return length16;
}



TextButton& OscillatorModule::getSinButton()
{
    return sinButton;
}


TextButton& OscillatorModule::getSawButton()
{
    return sawButton;
}


TextButton& OscillatorModule::getTriangleButton()
{
    return triButton;
}


TextButton& OscillatorModule::getSquareButton()
{
    return sqrButton;
}


void OscillatorModule::setOscMult( float v )
{
    oscMult = v;
}


float OscillatorModule::getOscMult( )
{
    return oscMult;
}


String OscillatorModule::getOscType()
{
    return oscType;
}
