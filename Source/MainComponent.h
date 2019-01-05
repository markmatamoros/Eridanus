/*
  ==============================================================================

    MainComponent.cpp
    Author:  Mark Matamoros
    
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomComponent.h"

// #include "KeyboardScene.h"

#include "InputModule.h"
#include "OscillatorModule.h"
#include "AmpFilterModule.h"
#include "EnvFilterModule.h"
#include "LFOModule.h"
#include "SaturationModule.h"
#include "OutputModule.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


class MainComponent   : // public Component,
                        // public AudioSource,
                        public AudioAppComponent,
                        public Slider::Listener,
                        public Button::Listener,
                        private MidiInputCallback,
                        private MidiKeyboardStateListener,
                        private ComboBox::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    //handles value changes in sliders
    void sliderValueChanged (Slider* slider) override
    {
        //osc 1 level
        if ( OSC_1->isLevelSlider( slider ) )
        {
            OSC_1->setLevelSliderValue( OSC_1->getLevelSliderValue() );
        }
        
        //osc 1 frequency fine tune
        else if ( OSC_1->isFineTuneSlider( slider ) )
        {
            OSC_1->setFineTuneSliderValue( OSC_1->getFineTuneSliderValue() );
        }
        
        //osc 2 level
        else if ( OSC_2->isLevelSlider( slider ) )
        {
            OSC_2->setLevelSliderValue( OSC_2->getLevelSliderValue() );
        }
        
        //osc 2 frequency fine tune
        else if ( OSC_2->isFineTuneSlider( slider ) )
        {
            OSC_2->setFineTuneSliderValue( OSC_2->getFineTuneSliderValue() );
        }
        
        //amp lfo rate (freq)
        else if ( LfoAmp->isRateSlider( slider ) ) 
        {
            LfoAmp->setRateSliderValue( LfoAmp->getRateSliderValue() );
        }
        
        //amp lfo depth
        else if ( LfoAmp->isDepthSlider( slider ) )
        {
            LfoAmp->setDepthSliderValue( LfoAmp->getDepthSliderValue() );
        }
 
        //saturation drive 
        else if ( saturation->isDriveSlider( slider ) )
        {
            saturation->setDriveSliderValue( saturation->getDriveSliderValue() );
        }
        
        //synth output
        else if ( output->isOutputSlider( slider ) )
        {
            output->setOutputSliderValue( output->getOutputSliderValue() );
        }
    }
  
    //function handles slider changes to osc and lfo frequencies
    void updateOscOneFrequency();
    void updateOscTwoFrequency();
    void updateLFOAmpFrequency();
    void updateLFOFreqFrequency();

    //create wavetables for osc and lfo's
    void createWavetables();
    
    //function handles user selected filter type adjustments
    void comboBoxChanged(ComboBox*) override;
    
    //function handles saturation selection
    void buttonClicked(Button*) override;
    
    //updates changes within filter settings
    void updateFilterSettings();
    
    //handle amp envelope manipulation
    void ampEnvelope();
    
    //handle filter envelope manipulation
    void filterEnvelope();
  
    //function receives incoming MIDI messages (Midi input device & Midi message argruments)
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override
    { 
        const ScopedValueSetter<bool> scopedInputFlag ( isAddingFromMidiInput, true );
        keyboardState.processNextMidiEvent ( message );
        // postMessageToList ( message, source->getName() );
 
        updateMidiNoteInformation ( message );

        //trigger envelopes
        if (message.isNoteOn())
        {
            smoothStart = true;     //handles smooth Amp beginning for osc's and env's
            
            //values trigger AMP envelope-state value generation within movement
            attackAmpEnv = true;
            decayAmpEnv = false;
            releaseAmpEnv = false;
            
            //values handle the beginning of the filter envelope
            attackFilEnv = true;
            releaseFilEnv = false;
        }
        
        //trigger the end of envelopes
        if (message.isNoteOff())
        {      
            //values trigger AMP envelope-state value generation within movement
            attackAmpEnv = false;
            decayAmpEnv = false;
            releaseAmpEnv = true;
            
            //values hand the release of the filter envelope
            attackFilEnv = false;
            releaseFilEnv = true;
        }
   };
    
    //select MIDI input device for listening
    void setMidiInputDevice (int index)
    {
        //store list (String Array) of available MIDI devices
        auto deviceList = MidiInput::getDevices();

        deviceManager.removeMidiInputCallback (deviceList[lastInputIndex], this );
        
        //store MIDI device (3rd option [2] for connected controller)
        auto newMidiDeviceInput = deviceList[index];

        //enable MIDI input device (controller)
        if ( ! deviceManager.isMidiInputEnabled (newMidiDeviceInput) ) {
            deviceManager.setMidiInputEnabled ( newMidiDeviceInput, true );
        }

        //set manager to receive all incoming events from the enabled device
        // deviceManager.addMidiInputCallback(newMidiDeviceInput, this);
        deviceManager.addMidiInputCallback (newMidiDeviceInput, this );
        midiInputList.setSelectedId (index + 1, dontSendNotification);

        lastInputIndex = index;
    }


    //
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        if ( ! isAddingFromMidiInput ) {
            auto m = MidiMessage::noteOn ( midiChannel, midiNoteNumber, velocity );
            m.setTimeStamp ( Time::getMillisecondCounterHiRes() * 0.001 );
            // postMessageToList ( m, "On-Screen Keyboard" );

            updateMidiNoteInformation ( m );
            
            smoothStart = true;     //handles smooth Amp beginning for osc's and env's
            
            //values trigger AMP envelope-state value generation within movement
            attackAmpEnv = true;
            decayAmpEnv = false;
            releaseAmpEnv = false;
            
            //values handle the beginning of the filter envelope
            attackFilEnv = true;
            releaseFilEnv = false;
        }
    }

    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
        if ( ! isAddingFromMidiInput ) {
            auto m = MidiMessage::noteOff ( midiChannel, midiNoteNumber );
            m.setTimeStamp ( Time::getMillisecondCounterHiRes() * 0.001 );
            updateMidiNoteInformation ( m );
            // postMessageToList ( m, "On-Screen Keyboard" );
            
            //values trigger AMP envelope-state value generation within movement
            attackAmpEnv = false;
            decayAmpEnv = false;
            releaseAmpEnv = true;
            
            //values hand the release of the filter envelope
            attackFilEnv = false;
            releaseFilEnv = true;
        }
    }

    void updateMidiNoteInformation (const MidiMessage& message)
    {
        //convert MIDI note to frequency for OSC's
        midiFrequency = (float) MidiMessage::getMidiNoteInHertz (message.getNoteNumber());
        
        //store MIDI note velocity for OSCs' levels
        midiVelocity = message.getFloatVelocity();
    }

private:
    //==============================================================================
    
    //Manages the state (properties) of Audio/MIDI devices
    AudioDeviceManager deviceManager;

    // MIDI INPUTS
    ComboBox midiInputList;
    Label    midiInputListLabel;
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;
 
    //Store MIDI Note On Frequency and Velocity
    float midiFrequency;
    float midiVelocity;
    
    float ampEnvValue = 1.0f;       //hold target amplitude envelope values
    
    //hold AMP bool values for triggering attack, decay, and release
    bool attackAmpEnv = false;     
    bool decayAmpEnv = false;
    bool releaseAmpEnv = false;

    //hold AMP bool values for triggering smooth Attacks and Releases (osc's and envelope)
    bool smoothStart = false;
    bool smoothStartFlag = false;
    
    float cutoff = 20.0f;           //hold filter cutoff value
    
    //hold FILTER bool values for triggering Attack, Decay, and Release
    bool attackFilEnv = false;
    bool releaseFilEnv = false;
    bool startFilEnv = false;
    bool endFilEnv = false;
    bool notFilDecay = false;
    
    float filEnvValue = 0.0f;       //hold target filter envelope values
    
    // ONSCREEN KEYBOARD
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;

    // SCENE COMPONENTS
    OwnedArray<Component> modules;
    OwnedArray<Component> scenes;
    

    // INPUT MODULE
    InputModule* Input;
    

    // LFO FREQ MODULE
    LfoModule* LfoFreq;
    String lfoFM = "FM LFO";
    

    // OSCILLATOR 1 & 2
    OscillatorModule* OSC_1;
    OscillatorModule* OSC_2;

          
    // LFO AMP
    LfoModule* LfoAmp;
    String lfoAmp = "AMP LFO";
    

    // AMP FILTER
    AmpFilterModule* ampFilter;
  

    // ENV FILTER
    EnvFilterModule* envFilter;


    // SATURATION MODULE
    SaturationModule* saturation;
    int saturationType;
   

    // OUTPUT MODULE
    OutputModule *output;

    //holds audio device's sample rate for osc, lfo, and filter settings
    float globalSampleRate;  
 
    double oscTableSize;    //size of wavetables (osc Arrays)

    double oscOneFrequency;    //osc frequency 
    double oscOnePhase;        //position within the osc wavetable cycle
    double oscOneIncrement;    //holds "next" place in wavetable for grabbing values
    
    double oscTwoFrequency;    //osc frequency 
    double oscTwoPhase;        //position within the osc wavetable cycle
    double oscTwoIncrement;    //holds "next" place in wavetable for grabbing values
       
    double lfoAmpFrequency;    //lfo Amp frequency 
    double lfoAmpPhase;        //position within the lfo Amp wavetable cycle
    double lfoAmpIncrement;    //position within the lfo Amp wavetable cycle
    
    double lfoFreqFrequency;    //lfo FM frequency 
    double lfoFreqPhase;        //position within the lfo FM wavetable cycle
    double lfoFreqIncrement;    //position within the lfo FM wavetable cycle
    
    //data structure (JUCE's array class, similar to vector) holds osc wavetables
    Array<float> sineTable;
    Array<float> squareTable;
    Array<float> sawTable;
    Array<float> triangleTable;
   
    //hold oscillators' wavetables for reading/output
    Array<float> wavetableOne;
    Array<float> wavetableTwo;
    

    //Filter creation...
    //Converts mono processor into multi-channels (2), as opposed to processing independently (L and R)
    //Comprised of two components: the filter (1rst part: contains mono processor)
    //and the filter's parameters (2nd part: state types (parameters)
    dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, 
         dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
 
    //handles the smoothing of fader values -> linear increments to target slider value      
    LinearSmoothedValue<float> smoothOsc1Level;
    LinearSmoothedValue<float> smoothOsc2Level;
    LinearSmoothedValue<float> smoothLFOAmpDepth;
    LinearSmoothedValue<float> smoothAmpEnv;
    LinearSmoothedValue<float> smoothFilEnv;
    LinearSmoothedValue<float> smoothDrive;
    LinearSmoothedValue<float> smoothOutput;
         
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
