/*
  ==============================================================================

    MainComponent.cpp
    Author:  Mark Matamoros
    
  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() 
    : keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{    
    // INPUT
    Input = new InputModule( );
    modules.add ( Input );
    
    // LFO FREQ
    LfoFreq = new LfoModule ( lfoFM );
    modules.add ( LfoFreq );
    
    // OSCILLATORS
    OSC_1 = new OscillatorModule ( "OSC 1" );
    modules.add( OSC_1 );
    
     // OSCILLATORS
    OSC_2 = new OscillatorModule ( "OSC 2" );
    modules.add( OSC_2 );
    
    // LFO AMP
    LfoAmp = new LfoModule( lfoAmp ); 
    modules.add ( LfoAmp );
   

    // ENV FILTER
    envFilter = new EnvFilterModule();
    modules.add ( envFilter );
  

    // AMP FILTER
    ampFilter = new AmpFilterModule();
    modules.add( ampFilter );
   

    // SATURATION
    saturation = new SaturationModule ();
    modules.add ( saturation );
   

    // OUTPUT
    output = new OutputModule();
    modules.add ( output );

    int numModules = modules.size();
    for (int i = 0; i < numModules; i++) {
        addAndMakeVisible ( modules[i] );
    }

    addAndMakeVisible ( keyboardComponent );
    keyboardComponent.setKeyWidth( 75.0f );
    keyboardComponent.setScrollButtonWidth( 30.0f );
    
    keyboardState.addListener ( this );

    addAndMakeVisible ( midiInputListLabel );
    midiInputListLabel.setText ("MIDI Input", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, false);
    midiInputListLabel.setJustificationType ( Justification::horizontallyCentred );

    addAndMakeVisible( midiInputList );
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    auto midiInputs = MidiInput::getDevices();
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.onChange = [this] {
        setMidiInputDevice ( midiInputList.getSelectedItemIndex() );
    };

    for (auto midiInput : midiInputs) {
        if (deviceManager.isMidiInputEnabled (midiInput)) {
            setMidiInputDevice ( midiInputs.indexOf (midiInput) );
            break;
        }
    }

    // IF NO DEVICE FOUND, USE THE FIRST ONE IN THE LIST
    if (midiInputList.getSelectedId() == 0) {
        setMidiInputDevice ( 0 );
    }
    
    // COMPONENT LISTENERS
    envFilter->getFilterButton( LOWPASS  ).addListener( this );
    envFilter->getFilterButton( HIGHPASS ).addListener( this );
    envFilter->getFilterButton( BANDPASS ).addListener( this );

    OSC_1->getLength4Button().addListener( this );
    OSC_1->getLength8Button().addListener( this );
    OSC_1->getLength16Button().addListener( this );
    OSC_1->getSinButton().addListener( this );
    OSC_1->getSawButton().addListener( this );
    OSC_1->getTriangleButton().addListener( this );
    OSC_1->getSquareButton().addListener( this );
    
    OSC_2->getLength4Button().addListener( this );
    OSC_2->getLength8Button().addListener( this );
    OSC_2->getLength16Button().addListener( this );
    OSC_2->getSinButton().addListener( this );
    OSC_2->getSawButton().addListener( this );
    OSC_2->getTriangleButton().addListener( this );
    OSC_2->getSquareButton().addListener( this );
    
    saturation->getTapeButton().addListener( this );
    saturation->getTubeButton().addListener( this );


    //set window size
    setSize (1600, 800);

    // Set MIDI device for listening (controller = [2])
    setMidiInputDevice( 2 );
    
    //set audio channels to 0 Inputs and 2 Outputs(Stereo playback)
    setAudioChannels (0, 2);

    // SET WAVETABLES
    String text = OSC_1->getOscType();
    if (text == "Sin")
        wavetableOne = sineTable;

    else if (text == "Saw")
        wavetableOne = sawTable;

    else if (text == "Tri")
        wavetableOne = triangleTable;

    else if (text == "Sqr")
        wavetableOne = squareTable;


    text = OSC_2->getOscType();
    if (text == "Sin")
        wavetableTwo = sineTable;

    else if (text == "Saw")
        wavetableTwo = sawTable;

    else if (text == "Tri")
        wavetableTwo = triangleTable;

    else if (text == "Sqr")
        wavetableTwo = squareTable;

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}


//prepareToPlay happens once when application is launched 
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{        
    //grab sampleRate for filter settings
    globalSampleRate = sampleRate; 
    
    //grab osc and lfo slider frequency values 
    oscOneFrequency = OSC_1->getFreqSliderValue(); 
    oscTwoFrequency = OSC_2->getFreqSliderValue();     
    lfoFreqFrequency = LfoAmp->getRateSliderValue();        
    lfoAmpFrequency = LfoAmp->getRateSliderValue(); 

    //set for starting at beginning of osc and lfo wavetables
    oscOnePhase = 0;
    oscTwoPhase = 0;
    lfoAmpPhase = 0;
    lfoFreqPhase = 0;
    
    //set wavetable size
    oscTableSize = 1024;

    //Determine increment size to find the next point in the osc wavetable for grabbing values 
    //Note: initialized here, but modifiable via functions 
    oscOneIncrement = oscOneFrequency * oscTableSize / globalSampleRate;
    oscTwoIncrement = oscTwoFrequency * oscTableSize / globalSampleRate;    
    lfoAmpIncrement = lfoAmpFrequency * oscTableSize / globalSampleRate;
    lfoFreqIncrement = lfoFreqFrequency * oscTableSize / globalSampleRate;
    
    //generate wavetables for osc and lfo's
    createWavetables();
    
    //relay settings (sample rate and block size) to dsp object (filter) 
    dsp::ProcessSpec spec;
    spec.sampleRate = globalSampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    
    //clear out garbage values
    stateVariableFilter.reset();    
        
    //initialize filter 
    stateVariableFilter.prepare(spec);

    //initialize values for LinearSmoothedValue Class
    //Sample rate & Time to reach set value (slider)
    smoothOsc1Level.reset(sampleRate, 0.035);
    smoothOsc1Level.setValue(0.0);
    
    smoothOsc2Level.reset(sampleRate, 0.035);
    smoothOsc2Level.setValue(0.0);
    
    smoothLFOAmpDepth.reset(sampleRate, 0.035);
    smoothLFOAmpDepth.setValue(0.0);

    smoothAmpEnv.reset(sampleRate, 0.035);
    smoothAmpEnv.setValue(0.0);
    
    smoothFilEnv.reset(sampleRate, 0.035);
    smoothFilEnv.setValue(0.0);
    
    smoothDrive.reset(sampleRate, 0.035);
    smoothDrive.setValue(0.0);
    
    smoothOutput.reset(sampleRate, 0.035);
    smoothOutput.setValue(0.0);
}


//audio-processing - AudioSouceChannel -> where to get buffer and channel information
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) 
{
    //grab osc and lfo depth slider values
    auto oscOneLevel = (float) OSC_1->getLevelSliderValue();
    auto oscTwoLevel = (float) OSC_2->getLevelSliderValue();
    auto lfoAmpDepth = (float) LfoAmp->getDepthSliderValue();
    auto drive = (float) saturation->getDriveSliderValue();
    auto outputValue = (float) output->getOutputSliderValue();
        
    //handles smooth beginning for envelopes and oscillators (eliminate audible clicks)
    if (smoothStart)
    {
        //handles the beginning of the envelope and oscillator amplitude 
        if (smoothStartFlag == false)
        {   
            //momentarily grab smoothed value from current state in Amp Envelope
            smoothAmpEnv.setValue(smoothAmpEnv.getNextValue()); 
            
            //reset AMP envelope smoothed liear class to "short" time-to-reach target
            smoothAmpEnv.reset(globalSampleRate, 0.04);
            
            //reset oscs' smoothed liear classes to "short" time-to-reach target
            smoothOsc1Level.reset(globalSampleRate, 0.04);
            smoothOsc2Level.reset(globalSampleRate, 0.04);
            
            //ensure that this reset point is not retriggered
            smoothStartFlag = true;
        }
        
        //set all target values to 0 (reached in 0.04 seconds)
        smoothAmpEnv.setValue(0.0f); 
        smoothOsc1Level.setValue(0.0);
        smoothOsc2Level.setValue(0.0);
                       
        //repeat smoothing until target value is reached, then...
        if (smoothAmpEnv.getNextValue() == 0.0f)
        {
            //...reset "smooth start" until next utilization of the amp envelope 
            smoothStart = false;
            smoothStartFlag = false;
        }
    }
    
    //set osc level mults during smooth start (reaching "target 0")
    if (smoothStart)
    {
        oscOneLevel = smoothOsc1Level.getNextValue(); 
        oscTwoLevel = smoothOsc2Level.getNextValue();
    }
    
    //when smoothStart is not active, set osc levels with slider values
    if (!smoothStart)
    {
        smoothOsc1Level.setValue(OSC_1->getLevelSliderValue());
        smoothOsc2Level.setValue(OSC_2->getLevelSliderValue());
    }

    //grab smoothed values if smooth start not active and the osc's smoothed class is active (until slider value is reached)
    if (!smoothStart && smoothOsc1Level.isSmoothing())
    {         
        oscOneLevel = smoothOsc1Level.getNextValue(); 
        oscTwoLevel = smoothOsc2Level.getNextValue();
    }
    
    saturationType = saturation->getSaturationType();
 
    //set target values for smooth class via slider values
    smoothLFOAmpDepth.setValue(lfoAmpDepth);
    smoothDrive.setValue(drive);
    smoothOutput.setValue(outputValue);
        
    //create pointers to beginning of both channels' buffer (for writing)
    auto* channelOne = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* channelTwo = bufferToFill.buffer->getWritePointer( 1, bufferToFill.startSample);
    
    //loop with conditional statements handles osc selection (from wavetables) for writing to buffer
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        //grab the lfo (amp) phase position
        float lfoAmpPhasePosition = sineTable[(int)lfoAmpPhase];
        
       //For the following osc's, grab the sample (from wavetable) in accordance to current phase 
       //Multiply against the osc level slider to determine amplitude
       channelOne[sample] = (wavetableOne[(int)oscOnePhase] * oscOneLevel) + (wavetableTwo[(int)oscTwoPhase] * oscTwoLevel);
       channelTwo[sample] = (wavetableOne[(int)oscOnePhase] * oscOneLevel) + (wavetableTwo[(int)oscTwoPhase] * oscTwoLevel);       
       
       //Check for LFO Amp Depth Smoothing
        if (smoothLFOAmpDepth.isSmoothing())
        {
            lfoAmpDepth = smoothLFOAmpDepth.getNextValue();        
        }

        //LFO Treatment - Wet to Dry Ratio 
        channelOne[sample] = ((channelOne[sample]) * (lfoAmpDepth * lfoAmpPhasePosition)) + 
                                ((channelOne[sample]) * (1.0 - lfoAmpDepth));
        channelTwo[sample] = ((channelTwo[sample]) * (lfoAmpDepth * lfoAmpPhasePosition)) + 
                                ((channelTwo[sample]) * (1.0 - lfoAmpDepth));  

       //update osc and lfo frequencies
       updateOscOneFrequency( );
       updateOscTwoFrequency( );
       updateLFOAmpFrequency( );
       updateLFOFreqFrequency();
    }
     
    //grab filled buffer and store in sample block (so we can use with the filter)
    dsp::AudioBlock<float> sampleBlock (*bufferToFill.buffer);

    //begin filter envelope value creation
    filterEnvelope();
    
    //grab filter parameter settings
    updateFilterSettings();
    
    //process sample block (buffer) with the filter
    stateVariableFilter.process(dsp::ProcessContextReplacing<float> (sampleBlock));
    
    if (!smoothStart)
    {
        ampEnvelope();
        
        smoothStartFlag = false;
    }
        
    //amp envelope processing    
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {   
        channelOne[sample] = channelOne[sample] * smoothAmpEnv.getNextValue(); 
        channelTwo[sample] = channelTwo[sample] * smoothAmpEnv.getNextValue();
    }
    
    //saturation processing
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        //check for drive knob smoothing
        if (smoothDrive.isSmoothing())
        {
            drive = smoothDrive.getNextValue();             
        }
        
        //Tube - 2nd Harmonic - Based on C. Dodge's "Computer Music" p. 147
        if (saturationType == 2)
        {
            channelOne[sample] = (channelOne[sample] * 0.9) + (drive * ((2 * channelOne[sample]) * (channelOne[sample] - 1)));              
            channelTwo[sample] = (channelTwo[sample] * 0.9) + (drive * ((2 * channelTwo[sample]) * (channelTwo[sample] - 1)));
        }   
        else //Tape - 3rd Harmonic - Based on C. Dodge's "Computer Music" p. 147
        {       
            channelOne[sample] = (channelOne[sample] * 0.9) + (drive * ((4 * channelOne[sample] * channelOne[sample] * channelOne[sample]) - (3 * channelOne[sample])));              
            channelTwo[sample] = (channelTwo[sample] * 0.9) + (drive * ((4 * channelTwo[sample] * channelTwo[sample] * channelTwo[sample]) - (3 * channelTwo[sample])));
        }
      
        //handle float values above 1 or below -1
        if (channelOne[sample] > 1.0f) 
            {channelOne[sample] = 1.0f;}
                
        if (channelOne[sample] < -1.0f) 
            {channelOne[sample] = -1.0f;}
                
        if (channelTwo[sample] > 1.0f) 
            {channelTwo[sample] = 1.0f;}
                
        if (channelTwo[sample] < -1.0f) 
            {channelTwo[sample] = -1.0f;}
    }    
       
    //output processing
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        //check for smoothed output values
        if(smoothOutput.isSmoothing())
        {
            outputValue = smoothOutput.getNextValue(); 
        }
        
        //calculate output via slider
        channelOne[sample] = ((channelOne[sample]) * (outputValue)); 
        channelTwo[sample] = ((channelTwo[sample]) * (outputValue));
    }
}

//When the audio device stops or restarted
void MainComponent::releaseResources() 
{    
    Logger::getCurrentLogger()->writeToLog ("Releasing resources");
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    Grid grid;
    
    using Track = Grid::TrackInfo;

    grid.autoRows = {
        Track (1_fr)
    };

    grid.autoColumns = {
        Track (1_fr)
    };
    
    grid.items = {
        GridItem ( LfoFreq           ).withArea( 1 , 1  , 9 , 10  ),
        GridItem ( OSC_1             ).withArea( 1 , 10 , 5 , 30  ),
        GridItem ( OSC_2             ).withArea( 5 , 10 , 9 , 30  ),
        GridItem ( LfoAmp            ).withArea( 1 , 30 , 9 , 40  ),
        GridItem ( envFilter         ).withArea( 1 , 40 , 9 , 70  ),
        GridItem ( ampFilter         ).withArea( 1 , 70 , 9 , 90  ),
        GridItem ( saturation        ).withArea( 1 , 90 , 9 , 100 ),
        GridItem ( output            ).withArea( 1 , 100, 9 , 110 ),
        GridItem ( midiInputList     ).withArea( 10, 1  , 10, 19  ),
        GridItem ( keyboardComponent ).withArea( 9 , 20 , 15, 110 ) 
    };
      
    Rectangle<int> bounds = getLocalBounds();
    
    grid.performLayout ( bounds );

}

//Handles combo box changes - grab selection's text
void MainComponent::comboBoxChanged(ComboBox* box)
{
    String text = box->getText();
}

//Handle button click events - set appropriate saturation function
void MainComponent::buttonClicked(Button* button)
{
    String text = button->getButtonText();
    OscillatorModule *currentOSC = nullptr;
    Array<float> *currentTable = nullptr;

    // DETERMINE OSCILLATOR
    if ( OSC_1->isThis( button ) ) {
        currentOSC = OSC_1;
        currentTable = &wavetableOne;
    }

    else if ( OSC_2->isThis( button ) ) {
        currentOSC = OSC_2;
        currentTable = &wavetableTwo;
    }

    if ( text == "4'" || text == "8'" || text == "16'" ) {
        currentOSC->lengthButtonClicked( button );
    }

    // THIS SHOULD BE REFACTORED
    if ( text == "Sin" || text == "Saw" || text == "Tri" || text == "Sqr" ) {
        currentOSC->waveButtonClicked( button );

        if (text == "Sin")
            *currentTable = sineTable;

        else if (text == "Saw")
            *currentTable = sawTable;

        else if (text == "Tri")
            *currentTable = triangleTable;

        else
            *currentTable = squareTable;
    }

    //set saturation type via text
    if ( text == "TB" || text == "TP" )
        saturation->buttonUpdate( text );

    //handles changes to filter type
    if (text  == "LP" || text == "BP" || text == "HP")
    {
        envFilter->setFilter( text );
    }
}




//Updates filter parameters set by user 
void MainComponent::updateFilterSettings()
{
    //grab and store filter slider values
    auto cutoff = (float) envFilter->getCutoffKnobValue();
    auto resonance = (float) envFilter->getResonanceKnobValue(); 
    
    //grab oscillator filter type
    String filterType = envFilter->getFilterType();

    //handles band pass selection in combo box
    if (filterType == "Band Pass")
    {
        //set filter type to low pass and set cutoff and resonance parameters
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency (globalSampleRate, cutoff * smoothFilEnv.getNextValue() + 20, resonance);
    }
    
    //handles high pass selection in combo box
    else if (filterType == "High Pass")
    {
        //set filter type to low pass and set cutoff and resonance parameters
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency (globalSampleRate, cutoff * (1.0f - smoothFilEnv.getNextValue()) + 20, resonance);
    }
    //handles low pass selection in combo box
    else
    {
        //set filter type to low pass and set cutoff and resonance parameters
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency (globalSampleRate, cutoff * smoothFilEnv.getNextValue() + 20, resonance);
    }  
}

//Handles user changes in osc frequency from slider
void MainComponent::updateOscOneFrequency()
{  
    //Determines even temperement tuning amount (one twelth movement based on octave below frequency) 
    auto fineTune = (((midiFrequency * OSC_1->getOscMult()) / 2) / 12) * OSC_1->getFineTuneSliderValue();
    
    //Determine frequency addition/subtraction based on FM lfo phase and set depth value
    auto freqModAdd = sineTable[(int)lfoFreqPhase] * (LfoFreq->getDepthSliderValue()*100);
    
    //Determine next point in the osc wavetable for grabbing values
    oscOneIncrement = ((midiFrequency * OSC_1->getOscMult()) + fineTune + freqModAdd) * oscTableSize / globalSampleRate;
    
    //"phase + increment" determines next part (phase) of osc table for grabbing values
    //"fmod" handles reaching past the osc table, and wrapping around to the appropriate phase from the beginning
    oscOnePhase = fmod((oscOnePhase + oscOneIncrement), oscTableSize);
}

//Handles user changes in osc Two frequency from slider
void MainComponent::updateOscTwoFrequency()
{
    //same as above
    auto fineTune = (((midiFrequency * OSC_2->getOscMult()) / 2) / 12) * OSC_2->getFineTuneSliderValue();
    
    auto freqModAdd = sineTable[(int)lfoFreqPhase] * (LfoFreq->getDepthSliderValue()*100);
    
    oscTwoIncrement = ((midiFrequency * OSC_2->getOscMult()) + fineTune + freqModAdd) * oscTableSize / globalSampleRate;
    
    oscTwoPhase = fmod((oscTwoPhase + oscTwoIncrement), oscTableSize);
}

//Handles user changes in lfo Amp frequency from slider
void MainComponent::updateLFOAmpFrequency()
{
    //Determine next point in the lfo Amp wavetable for grabbing values
    lfoAmpIncrement = LfoAmp->getRateSliderValue() * oscTableSize / globalSampleRate;
    
    //"phase + increment" determines next part (phase) of lfo Amp table for grabbing values
    //"fmod" handles reaching past the lfo table, and wrapping around to the appropriate phase from the beginning
    lfoAmpPhase = fmod((lfoAmpPhase + lfoAmpIncrement), oscTableSize);
}

//Handles user changes in lfo FM frequency from slider
void MainComponent::updateLFOFreqFrequency()
{
    //Determine next point in the lfo FM wavetable for grabbing values
    lfoFreqIncrement = (LfoFreq->getRateSliderValue() * 2) * oscTableSize / globalSampleRate;
    
    //"phase + increment" determines next part (phase) of lfo FM table for grabbing values
    //"fmod" handles reaching past the lfo FM table, and wrapping around to the appropriate phase from the beginning
    lfoFreqPhase = fmod((lfoFreqPhase + lfoFreqIncrement), oscTableSize);
}

//create wavetables for sine, sawtooth, square, and triangle
void MainComponent::createWavetables()
{
    //for loop handles calculations for inserting appropriate values into the table, sample to sample
    for (int i = 0; i < oscTableSize; i++)
    {
        //sine wave table for osc and amp lfo
        sineTable.insert(i , sin(2.0 * double_Pi * i / oscTableSize));   
        
        //saw wave table for osc (ramp up)
        sawTable.insert(i , ((i / oscTableSize) - 0.5) * 2);   
        
        //square wave table for osc
        if (i / oscTableSize < 0.5)     //first half of table (-1)
        {
            squareTable.insert(i, -1);
        }
        else                            //second half of table (1)
        {
            squareTable.insert(i, 1);
        }
        
        //triangle wave table for osc
        if (i / oscTableSize < 0.5)     //first half of table (ramp up)
        {
            triangleTable.insert(i , ((i / (oscTableSize/2)) - 0.5) * 2); 
        }
        else                            //second half ot table (ramp down)
        {
            triangleTable.insert(i, (0.5 - ((i - oscTableSize/2) / (oscTableSize/2))) * 2);
        }
    }
}

void MainComponent::ampEnvelope()
{
    //beginning of envelope (Attack) value generation, triggered by MIDI note ON
    if (attackAmpEnv == true)
    {
        //set smoothed value class with Attack time
        smoothAmpEnv.reset(globalSampleRate, ampFilter->getAttackSliderValue() + 0.04);        

        //set bool's for continuation in envelope
        attackAmpEnv = false;
        decayAmpEnv = false;
        releaseAmpEnv = false;
        
        ampEnvValue = 1.0f;     //set target value
    }
    
    //set envelope to handle Decay values if target Attack value is reached
    if (smoothAmpEnv.getNextValue() == midiVelocity)
    {
        //set bool's for continuation in envelope
        attackAmpEnv = false;
        decayAmpEnv = true;
        releaseAmpEnv = false;
    }
    
    //set envelope to Decay
    if (decayAmpEnv == true)
    {
        //momentarily grab current value and set as target
        smoothAmpEnv.setValue(smoothAmpEnv.getNextValue());
        
        //reset linear smoothed class to Decay time
        smoothAmpEnv.reset(globalSampleRate, ampFilter->getDecaySliderValue() + 0.04);

        //set bool's for continuation in envelope
        attackAmpEnv = false;
        decayAmpEnv = false;
        releaseAmpEnv = false;
        
        //set target value, based on Sustain slider value
        ampEnvValue = ampFilter->getSustainSliderValue();
    }
    
    //set envelope to Release, triggered by MIDI Note Off
    if (releaseAmpEnv == true)
    {
        //momentarily grab current value and set as target
        smoothAmpEnv.setValue(smoothAmpEnv.getNextValue());
         
        //reset linear smoothed class to Release time
        smoothAmpEnv.reset(globalSampleRate, ampFilter->getReleaseSliderValue() + 0.04);

        //set bool's for continuation in envelope
        attackAmpEnv = false;
        decayAmpEnv = false;
        releaseAmpEnv = false; 
        
        //set target value
        ampEnvValue = 0.0f;        
    }
    
    //set target value vor linear smoothed class, based on MIDI velocity and target envelope values
    smoothAmpEnv.setValue(ampEnvValue * midiVelocity);
}

//creates envelope values for the filter's envelope (cutoff handling)
void MainComponent::filterEnvelope()
{
    //handles the attack, decay, and sustain values of the envelope, triggered by MIDI Note On
    if (attackFilEnv)
    {        
        //handles the beginning of the envelope 
        if (!startFilEnv)
        {    
            //set linear smoothed class with Attack time (to reach target)
            smoothFilEnv.reset(globalSampleRate, 0.01 * envFilter->getAttackSliderValue() + 0.0004);        
            
            startFilEnv = true;     //change bool to not reset the beginning of the envelope 
            endFilEnv = false;      //change bool to prep Release of the envelope (triggered by Note Off)
            notFilDecay = true;     //change bool to continue the full Attack amount
        }
        
        //Begin Decay if peak value is hit on the attack
        if (smoothFilEnv.getNextValue() == 1.0f)
        {
            //set linear smoothed class with Decay time (to reach target)
            smoothFilEnv.reset(globalSampleRate, 0.01 * envFilter->getDecaySliderValue() + 0.0004);
            
            //set target from Sustain slider value
            filEnvValue =  envFilter->getSustainSliderValue();
            
            //change bool to begin new decay
            notFilDecay = false;
        }        
        
        //Continue full attack amount until reaching peak value
        if (notFilDecay)
        {
            filEnvValue = 1.0;
        }
        
        //set target values for linear smooth class
        smoothFilEnv.setValue(filEnvValue);
    }
          
    //handles the release values of the envelope, triggered by MIDI Note Off
    else if (releaseFilEnv)
    {      
        //handles the Release of the envelope 
        if (!endFilEnv)
        {
            //momentarily set target value to the last smoothed envelope value
            smoothFilEnv.setValue(smoothFilEnv.getNextValue());        
            
            //reset smoothed value class to time based on the Sustain slider value
            smoothFilEnv.reset(globalSampleRate,  0.01 * envFilter->getReleaseSliderValue() + 0.0004);
                        
            endFilEnv = true;       //change bool to finalize target (not re-enter conditional statement)
            startFilEnv = false;    //change bool to prep the next beginning of the envelope
            
            //set target value
            smoothFilEnv.setValue(0.0);
        }          
    }
}