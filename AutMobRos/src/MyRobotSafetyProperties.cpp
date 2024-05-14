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
      seSystemOff("Shutdown the system")
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

    // Add events to individual safety levels
    slSystemOff.addEvent(seSystemOn, slSystemOn, kPublicEvent);
    slSystemOn.addEvent(seSystemOff, slSystemOff, kPublicEvent);
    slEmergency.


    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    // /!\ All critical inputs and outputs must be defined in EACH level
    slSystemOn.setInputActions({
        ignore(onBoardButtonMode), 
        check(onBoardButtonPause, false, seSystemOff)
    });
    slSystemOff.setInputActions({
        ignore(onBoardButtonMode), 
        ignore(onBoardButtonPause)
    });

    // Define output actions for all levels
    slSystemOn.setOutputActions({
        set(onBoardLEDgreen, true), 
        set(onBoardLEDred, true)
    });
    slSystemOff.setOutputActions({
        set(onBoardLEDgreen, false), 
        set(onBoardLEDred, false)
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

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(seSystemOff);
    });
}
