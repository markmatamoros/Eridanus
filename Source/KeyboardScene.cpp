/*
  ==============================================================================

    KeyboardScene.cpp
    Created: 26 Oct 2018 6:52:51am
    Author:  allen

  ==============================================================================
*/

#include "KeyboardScene.h"

/*
KeyboardScene::KeyboardScene ()
{
    setBaseFrequency( 440.0 );
    fillFrequecyVector( );

    lower = new Octave( getOctaveFrequencies( 2 ) );
    addAndMakeVisible( lower );
    
    upper = new Octave( getOctaveFrequencies( 3 ) );
    addAndMakeVisible( upper );
}


KeyboardScene::~KeyboardScene ()
{
    delete lower;
    delete upper;
}

void KeyboardScene::paint ( Graphics& g )
{
    g.setColour ( BORDER_COLOR );
}

void KeyboardScene::resized ()
{
    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows    = { 
        Track (1_fr)
    };
    
    grid.templateColumns = { 
        Track (1_fr), 
        Track (1_fr)
    };

    grid.autoColumns = Track (1_fr);

    grid.setGap( 10_px );


    grid.items = {
        GridItem ( lower ),
        GridItem ( upper )
    };

    Rectangle <int> bounds = getLocalBounds();


    bounds = moduleInternalsBounds ( bounds,
                                     MODULE_INSIDE_OFFSET,
                                     OFFSET,
                                     THICKNESS);

    grid.performLayout( bounds );
}


void KeyboardScene::fillFrequecyVector()
{

    int distFromA4 = -45,  
        numOctaves = 8,
        numKeys    = numOctaves * 12;

    float curBaseFrequency = getBaseFrequency();

    for (int i = 0; i < numOctaves; i++) {
        vector<float> currentOctave;

        for (int j = 0; j < 12; j++) {
            float curNoteFreq = pow( 2, (distFromA4 / 12) );
            curNoteFreq      *= curBaseFrequency;
            currentOctave.push_back( curNoteFreq );
            distFromA4++;
        }

        key_frequencies.push_back( currentOctave );
    }
}


float KeyboardScene::getBaseFrequency()
{
    return baseFrequency;
}


void KeyboardScene::setBaseFrequency( float v )
{
    if (v < 440.0 || v > 440.0) {
        printf("BASE FREQUENCY ERROR\n");
    }

    baseFrequency = v;
}


void KeyboardScene::addListeners( Button::Listener* l )
{
    return;
}


vector<float> KeyboardScene::getOctaveFrequencies( int octave )
{
    if (octave < 0 || octave > 8) {
        printf("FREQ OCTAVE ERROR");
        return key_frequencies.at( 0 );
    }

    return key_frequencies.at( octave );
}



KeyboardScene::Octave::Octave( vector<float> frequencies )
{
    int curIndex = 0;

    c1 =      new WhiteKey( frequencies.at( curIndex++ ) );
    cSharp1 = new BlackKey( frequencies.at( curIndex++ ) );
    d1 =      new WhiteKey( frequencies.at( curIndex++ ) );
    dSharp1 = new BlackKey( frequencies.at( curIndex++ ) );
    e1 =      new WhiteKey( frequencies.at( curIndex++ ) );
    f1 =      new WhiteKey( frequencies.at( curIndex++ ) );
    fSharp1 = new BlackKey( frequencies.at( curIndex++ ) );
    g1 =      new WhiteKey( frequencies.at( curIndex++ ) );
    gSharp1 = new BlackKey( frequencies.at( curIndex++ ) );
    a2 =      new WhiteKey( frequencies.at( curIndex++ ) );
    aSharp2 = new BlackKey( frequencies.at( curIndex++ ) );
    b2 =      new WhiteKey( frequencies.at( curIndex++ ) );
 
    if (curIndex != 12) {
        printf("KEY FREQ ASSIGN ERROR\n");
    }
 
    addAndMakeVisible( c1 );
    addAndMakeVisible( d1 );
    addAndMakeVisible( e1 );
    addAndMakeVisible( f1 );
    addAndMakeVisible( g1 );
    addAndMakeVisible( a2 );
    addAndMakeVisible( b2 );
 
    addAndMakeVisible( cSharp1 );
    addAndMakeVisible( dSharp1 );
    addAndMakeVisible( fSharp1 );
    addAndMakeVisible( gSharp1 );
    addAndMakeVisible( aSharp2 );
}


KeyboardScene::Octave::~Octave()
{
    delete c1;
    delete d1;
    delete e1;
    delete f1;
    delete g1;
    delete a2;
    delete b2;

    delete cSharp1;
    delete dSharp1;
    delete fSharp1;
    delete gSharp1;
    delete aSharp2;
}


void KeyboardScene::Octave::paint ( Graphics& g ) {}



void KeyboardScene::Octave::resized (  ) 
{
    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows    = { 
        Track (1_fr),
        Track (1_fr),
        Track (1_fr)
    };
    
    grid.templateColumns = { 
        Track (1_fr)
    };

    grid.autoColumns = Track (1_fr);

    grid.setGap( 10_px );


    grid.items = {
        GridItem ( c1      ).withArea( 1, 1,  4, 4 ),
        GridItem ( cSharp1 ).withArea( 1, 3,  3, 5 ),
        GridItem ( d1      ).withArea( 1, 4,  4, 7 ),
        GridItem ( dSharp1 ).withArea( 1, 6,  3, 8 ),
        GridItem ( e1      ).withArea( 1, 7,  4, 10 ),
        GridItem ( f1      ).withArea( 1, 10, 4, 13 ),
        GridItem ( fSharp1 ).withArea( 1, 12, 3, 14 ),
        GridItem ( g1      ).withArea( 1, 13, 4, 16 ),
        GridItem ( gSharp1 ).withArea( 1, 15, 3, 17 ),
        GridItem ( a2      ).withArea( 1, 16, 4, 19 ),
        GridItem ( aSharp2 ).withArea( 1, 18, 3, 20 ),
        GridItem ( b2      ).withArea( 1, 19, 4, 22 )
    };

    Rectangle <int> bounds = getLocalBounds();

    grid.performLayout ( bounds );
}




KeyboardScene::Octave::WhiteKey::WhiteKey( float freq )
{
    frequency = freq;

    whiteKey.setColour( TextButton::buttonColourId, Colours::ivory );
    whiteKey.setColour( TextButton::buttonOnColourId, Colours::green );

    addAndMakeVisible( whiteKey );
}


KeyboardScene::Octave::WhiteKey::~WhiteKey()
{
}



void KeyboardScene::Octave::WhiteKey::paint ( Graphics& g )
{
    g.setColour( Colours::ivory );

    // LookAndFeel_V4::drawButtonBackground( g, whiteKey, Colours::ivory, false, false );
    
    g.setColour( Colours::blue );
    g.drawText ( "WhiteKey", 0, 0, getWidth(), getHeight(), Justification::centred, false);
}


void KeyboardScene::Octave::WhiteKey::resized ( )
{

    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows = {
        Track (1_fr)
    };

    grid.templateColumns = {
        Track (1_fr)
    };

    
    grid.items = {
        GridItem ( whiteKey )
    };

    grid.performLayout( getLocalBounds() );

}


float KeyboardScene::Octave::WhiteKey::getFrequency ( )
{
    return frequency;
}


KeyboardScene::Octave::BlackKey::BlackKey( float freq )
{
    frequency = freq;
    addAndMakeVisible( blackKey ); 

    blackKey.setColour( TextButton::buttonColourId, Colours::black );
    blackKey.setColour( TextButton::buttonOnColourId, Colours::red );
}


KeyboardScene::Octave::BlackKey::~BlackKey()
{
}



void KeyboardScene::Octave::BlackKey::paint ( Graphics& g )
{
    g.setColour( Colours::black );

    // LookAndFeel_V4::drawButtonBackground( g, blackKey, Colours::black, false, false );

    g.setColour( Colours::blue );
    g.drawText ( "BlackKey", 0, 0, getWidth(), getHeight(), Justification::centred, false);
}


void KeyboardScene::Octave::BlackKey::resized ( )
{
    Grid grid;

    using Track = Grid::TrackInfo;

    grid.templateRows = {
        Track (1_fr)
    };

    grid.templateColumns = {
        Track (1_fr)
    };

    
    grid.items = {
        GridItem ( blackKey )
    };

    grid.performLayout( getLocalBounds() );
}


float KeyboardScene::Octave::BlackKey::getFrequency ( )
{
    return frequency;
}

*/
