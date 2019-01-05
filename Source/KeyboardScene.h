/*
  ==============================================================================

    KeyboardScene.cpp
    Created: 26 Oct 2018 6:52:51am
    Author:  allen

  ==============================================================================
*/

#pragma once

#include "CustomComponent.h"
// #include "Keys.h"



/*
struct SineWaveSound : public SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote    (int) override
    {
        cout << "SineWaveSound::appliesToNote" << endl;
        return true;
    }

    bool appliesToChannel (int) override
    {
        cout << "SineWaveSound::appliesToChannel" << endl;
        return true;
    }
};


struct SineWaveVoice   : public SynthesiserVoice
{
    SineWaveVoice() {}

    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
    }

    void stopNote (float /*velocity/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0) // [7]
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99; // [8]

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote(); // [9]

                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0) // [6]
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level);

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};


class SynthAudioSource   : public AudioSource
{
public:
    SynthAudioSource (MidiKeyboardState& keyState)
        : keyboardState (keyState)
    {
        for (auto i = 0; i < 4; ++i)                // [1]
            synth.addVoice (new SineWaveVoice());

        synth.addSound (new SineWaveSound());       // [2]
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate); // [3]
        midiCollector.reset (sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);       // [4]

        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples); // [5]
    }

    MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
    
    MidiKeyboardState& getKeyboardState()
    {
        return keyboardState;
    }

private:
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
};



class KeyboardScene : // public AudioAppComponent,
                       public CustomComponent,
                       private Timer
{
    public:
    KeyboardScene() 
        : synthAudioSource  (keyboardState),
          keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    {
        CustomComponent::addAndMakeVisible (keyboardComponent);

        startTimer ( 400 );
    }

    ~KeyboardScene() {}

    void resized() override
    {
        Grid grid;

        using Track = Grid::TrackInfo;

        grid.templateRows = {
            Track (1_fr)
        };

        grid.templateColumns = {
            Track (1_fr)
        };

        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        grid.items = {
            GridItem (keyboardComponent)
        };

        Rectangle <int> bounds = CustomComponent::getLocalBounds();

        bounds = moduleInternalsBounds ( bounds,
                                         MODULE_INSIDE_OFFSET,
                                         OFFSET,
                                         THICKNESS );

        grid.performLayout( bounds );
    }

    

    MidiMessageCollector* getMidiCollector()
    {
        return synthAudioSource.getMidiCollector();
    }
    
    MidiKeyboardState& getKeyboardState()
    {
        // return synthAudioSource.getKeyboardState();
        return keyboardState;
    }



    private:
    SynthAudioSource synthAudioSource;
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent keyboardComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardScene);
    
    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }
};





class KeyboardScene : public CustomComponent
{
public:
    KeyboardScene ();
    ~KeyboardScene ();

    void paint ( Graphics& g ) override;
    void resized () override;

    void fillFrequecyVector();
    float getBaseFrequency();
    void  setBaseFrequency( float v );
    vector<float> getOctaveFrequencies( int octave );
    
    void addListeners( Button::Listener*);

    class Octave : public CustomComponent 
    {
    public:
        Octave( vector<float> frequencies );
        ~Octave();

        void paint ( Graphics& g ) override;
        void resized () override;


        class WhiteKey : public Keys 
        {
        public:
            WhiteKey( float freq );
            ~WhiteKey();

            void paint ( Graphics& g );
            void resized () override;

            float getFrequency();

        private:
            TextButton whiteKey;
            float frequency;
        };
 

        class BlackKey : public Keys
        {
        public:
            BlackKey( float freq );
            ~BlackKey();

            void paint ( Graphics& g );
            void resized () override;

            float getFrequency();

        private:
            TextButton blackKey;
            float frequency;
        };

    private:

        WhiteKey* c1;
        BlackKey* cSharp1;
        WhiteKey* d1;
        BlackKey* dSharp1;
        WhiteKey* e1;
        WhiteKey* f1;
        BlackKey* fSharp1;
        WhiteKey* g1;
        BlackKey* gSharp1;
        WhiteKey* a2;
        BlackKey* aSharp2;
        WhiteKey* b2;

    };

   
private:

    Octave* lower;
    Octave* upper;

    float baseFrequency;
    // HOLDS FREQUENCIES FROM C0 thru B7
    vector < vector<float> > key_frequencies;

};
*/




