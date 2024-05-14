#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slSystemOn("System is online"),
      slEmergency("System is in emergency mode (stopped)"),

    //   slEmergencyStopping("System is emergency stopping"),

    //   slDriveEnabled("System drive power enabled"),
    //   slDriving("System Driving"),

      seSystemOn("Startup the system"),
      seSystemOff("Shutdown the system"),
      seEmergency("Transitioning into emergency state"),
      seResetEmergency("Transitioning out of emergency state")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    onBoardLEDgreen = hal.getLogicOutput("onBoardLEDgreen");
    onBoardLEDred = hal.getLogicOutput("onBoardLEDred");

    criticalOutputs = {onBoardLEDgreen, onBoardLEDred};

    // Declare and add critical inputs
    onBoardButtonMode = eeros::hal::HAL::instance().getLogicInput("onBoardButtonMode");
    onBoardButtonPause = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause");

    criticalInputs = {onBoardButtonMode, onBoardButtonPause};

    // Add all safety levels to the safety system (lowest to highest)
    addLevel(slSystemOff);
    addLevel(slSystemOn);
    addLevel(slEmergency);

    // Add events to multiple safety levels 
    //addEventToAllLevelsBetween(lowlevel, highlevel, event, targetLevel, kPublicEvent/kPrivateEvent);
    addEventToAllLevelsBetween(slSystemOff, slEmergency, seSystemOff, slSystemOff, kPublicEvent); // enable Ctr+C everywhere

    // Add events to individual safety levels
    slSystemOff.addEvent(seSystemOn, slSystemOn, kPublicEvent);

    //slSystemOn.addEvent(seSystemOff, slSystemOff, kPublicEvent);
    slSystemOn.addEvent(seEmergency, slEmergency, kPublicEvent);

    slEmergency.addEvent(seResetEmergency, slSystemOn, kPublicEvent);
    

    // Define input actions for all levels (check for signals that trigger safety events)
    // /!\ All critical inputs and outputs must be defined in EACH level
    slSystemOff.setInputActions({
        ignore(onBoardButtonMode), 
        ignore(onBoardButtonPause)
    });
    slSystemOn.setInputActions({
        ignore(onBoardButtonMode), 
        check(onBoardButtonPause, false, seEmergency)
    });
    slEmergency.setInputActions({
        check(onBoardButtonMode, false, seResetEmergency), 
        ignore(onBoardButtonPause)
    });


    // Define output actions for all levels
    slSystemOff.setOutputActions({
        set(onBoardLEDgreen, false), 
        set(onBoardLEDred, false)
    });
    slSystemOn.setOutputActions({
        set(onBoardLEDgreen, true), 
        set(onBoardLEDred, false)
    });
    slEmergency.setOutputActions({
        set(onBoardLEDgreen, false), 
        set(onBoardLEDred, true)
    });


    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
    });


    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function (Ctr+C)
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(seSystemOff);
    });
}
