/*
  ==============================================================================

    IdealBar.cpp
    Created: 29 Mar 2021 2:29:22pm
    Author:  Devansh

  ==============================================================================
*/

#include "IdealBar.h"

idealBar::idealBar(double length, double width, double height, double Fs, double E, double density, double dampNonFreq, double dampFreq){
    //Assign Length, Width and Height of the bar
    barLength = length;
    barWidth = width;
    barHeight = height;
    
    //Calculate Area and Moment of Inertia of the bar
    barArea = barWidth*barHeight;
    barMomentOfInertia = ((barWidth * pow(barHeight,3))/12);
    
    //Assign Young's Modulus and Density of the bar
    barE = E;
    barDensity = density;
    
    //Calculate stiffness coefficient of the bar
    stiffnessCoefficient = sqrt((barE*barMomentOfInertia)/(barDensity*barArea));
    
    //Assign non-frequency based and frequency based damping terms
    sigmaZero = dampNonFreq;
    sigmaOne = dampFreq;
    
    //Calculate time-step based on sampling frequency
    k = 1/Fs;
    
    //Calculate spatial step based on stability condition and length of the bar
    double factor = sqrt((pow(sigmaOne,2)) + pow(stiffnessCoefficient,2));
    h = sqrt((2*k*(sigmaOne + factor)));
    N = floor(barLength/h);
    h = barLength/N;
    
    //Calculate update equation constants
    MU = (stiffnessCoefficient*k)/(pow(h,2));
    c1 = 2*sigmaZero*k;
    c2 = (sigmaOne*k)/(h*h);
    
    initializeUpdateVectors();
    
}

idealBar::~idealBar(){
    
}

void idealBar::initializeUpdateVectors(){
    std::cout<<N;
    for (int i = 1; i<= N+1; i++){
        u.push_back(0);
        uNext.push_back(0);
        uPrev.push_back(0);
    }
}


void idealBar::strike(int hannWidth){
    //Initial Excitement - Raised Cosine
    for(int i = 0; i<hannWidth; i++){
        u[i+2] = 0.5 * (1-cos(2*double_Pi*((double)(i)/(double)(hannWidth-1))));
        uPrev[i+2] = 0.5 * (1-cos(2*double_Pi*((double)(i)/(double)(hannWidth-1))));
    }
}

void idealBar::updateOperation(){
    for (int range = 2; range < N-1; range++){
        //Standard conditions
        uNext[range] = ((2 - 6*MU*MU - c1 - 2*c2) * u[range])
                    + ((c1 + 2*c2 - 1) * uPrev[range])
                    - (MU*MU * (u[range+2] + u[range-2]))
                    + ((4*MU*MU +c2) * (u[range+1] + u[range-1]))
                    - (c2 * (uPrev[range+1] + uPrev[range-1]));
    }
       //Free boundary condition - left boundary 1
       /* uNext[0] = ((2 - 6*pow(MU,2) - c1 - 2*c2) * u[0])
                    + ((c1 + 2*c2 -1) * uPrev[0])
                    - (2*MU*MU * (u[2] - 2*u[1]));
        
        //Free boundary condition - left boundary 2
        uNext[1] = ((2 - 5*pow(MU,2) - c1 - 2*c2) * u[1])
                    + ((c1 + 2*c2 - 1) * uPrev[1])
                    - (pow(MU,2) * u[3])
                    + ((4*pow(MU,2) + c2) * (u[2] + u[0]))
                    - (c2 * (uPrev[2] + uPrev[0]));
        
        //Free boundary condition - right boundary 1
        uNext[N] = ((2 - 6*pow(MU,2) - c1 - 2*c2) * u[N])
                    + ((c1 + 2*c2 -1) * uPrev[N])
                    - (2*MU*MU * (u[N-2] - 2*u[N-1]));
        
        //Free boundary condition - right boundary 1
        uNext[N-1] = ((2 - 5*pow(MU,2) - c1 - 2*c2) * u[N-1])
                    + ((c1 + 2*c2 - 1) * uPrev[N-11])
                    - (pow(MU,2) * u[N-3])
                    + ((4*pow(MU,2) + c2) * (u[N] + u[N-2]))
                    - (c2 * (uPrev[N] + uPrev[N-2]));*/
}

void idealBar::stateChange(){
    std::vector<double> temp;
    for (int i=0; i<uPrev.size(); i++){
        temp.push_back(0);
    }
    
    /*for (int i = 0; i<uPrev.size() ; i++){
        temp[i] = uPrev[i];
        uPrev[i] = u[i];
        u[i] = uNext[i];
        uNext[i] = temp[i];
    }*/ //Do Pointer Switch
    
    temp = uPrev;
    uPrev = u;
    u = uNext;
    uNext = temp;
}

double idealBar::getSchemeOutput(int location){
    //std::cout<<u[location]<<std::endl;
    return(u[location]);
}

void idealBar::paint(juce::Graphics &g){
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Bar", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text}
    
    resized();
}

void idealBar::resized(){
    
}
 
void idealBar::mouseDown(const MouseEvent &e){
    strike(4);
}
 
