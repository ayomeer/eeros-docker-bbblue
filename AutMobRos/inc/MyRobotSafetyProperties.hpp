#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSystemOn;

    eeros::safety::SafetyLevel slEmergency;
    // eeros::safety::SafetyEvent slEmergencyStopping;

    // eeros::safety::SafetyEvent slDriveEnabled;
    // eeros::safety::SafetyEvent slDriving;

    // Define all possible events
    eeros::safety::SafetyEvent seSystemOff;
    eeros::safety::SafetyEvent seSystemOn;

    eeros::safety::SafetyEvent seEmergency;
    eeros::safety::SafetyEvent seResetEmergency;


private:
    // Define all critical inputs
    eeros::hal::Input<bool>* onBoardButtonMode; // reset emergency
    eeros::hal::Input<bool>* onBoardButtonPause; // emergency

    
    // Define all critical outputs
    eeros::hal::Output<bool>* onBoardLEDgreen;
    eeros::hal::Output<bool>* onBoardLEDred;

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
