#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    bar = std::make_unique<idealBar>(0.293, 0.037, 0.019, sampleRate, (9.54 * pow(10,9)), 796, 0.7, 0.005);
    addAndMakeVisible(bar.get());
    resized();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    //int totalChannels = bufferToFill.buffer->getNumChannels();
    float* outputL = bufferToFill.buffer->getWritePointer(0);
    float* outputR = bufferToFill.buffer->getWritePointer(1);
     
    float output = 0.0;
    
    for(int i =0; i<bufferToFill.numSamples; ++i){
        output = bar->getSchemeOutput(6);
        
        bar->updateOperation();
        bar->stateChange();
        
        output = limit(output);
        outputL[i] = output;
        outputR[i] = outputL[i];
    }
}
    
void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
      if (bar != nullptr)
        bar->setBounds (getLocalBounds());
}

double MainComponent::limit(double value){
    if(value>1)
        return 1;
    else if(value<-1)
        return -1;
    else
        return value;
}
