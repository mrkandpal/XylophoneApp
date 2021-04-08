/*
  ==============================================================================

    IdealBar.h
    Created: 29 Mar 2021 2:29:22pm
    Author:  Devansh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <stdio.h>
#include <math.h>
using namespace std;

class idealBar : public juce::Component{
public:
    //Constructor and Destructor
    idealBar(double length, double width, double height, double Fs, double E, double density, double dampNonFreq, double dampFreq);
    ~idealBar();
    
    //Set functions for constants of ideal bar
    void setBarDimensions(double yModulus, double density, double height);
    
   //Get Functions for constants of ideal bar
    double getYModulus();
    double getDenssity();
    double getstiffnessCoefficient();
    
    //GUI and Display Methods
    void paint(juce::Graphics &g) override;
    void resized() override;
    
    //Function to initialise update finite difference scheme of ideal bar
    void strike(int hannWidth);
    void initializeUpdateVectors();
    void updateOperation();
    void stateChange();
    
    //Function to read output of finite difference scheme at a given location
    double getSchemeOutput(int location);
    
    //Function to record mouse click
   void mouseDown(const MouseEvent &e) override;
    
private:
    //Initialise all data members for an ideal bar here
    
    //Dimensions of Ideal Bar
    double barLength, barWidth, barHeight, barArea, barMomentOfInertia;
    
    //Bar Constants
    double barE; //Young's Modulus of the bar
    double barDensity;
    double stiffnessCoefficient;
    
    //Damping Terms
    double sigmaZero;
    double sigmaOne;
    
    //FDS factors
    double h,k,N;
    
    //Vectors to store FDS states
    std::vector<double> u;
    std::vector<double> uNext;
    std::vector<double> uPrev;
    
    //Update Equation Constants
    float MU, c1, c2;

};
