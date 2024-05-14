#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : encoder("enc1"),  // NOTE: these strings are IDs as defined in
      motor("motor1"),  //        HwConfig json file, not just names
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    encoder.setName("Encoder");
    controller.setName("Controller");
    motor.setName("Motor");

    // Name all signals (tied to outputs)
    encoder.getOut().getSignal().setName("phi [rad]");
    controller.getOut().getSignal().setName("U [V]");

    // Connect signals
    controller.getIn().connect(encoder.getOut());
    motor.getIn().connect(controller.getOut());

    // Add blocks to main time domain 
    // (!) In order of execution!
    timedomain.addBlock(encoder);
    timedomain.addBlock(controller);
    timedomain.addBlock(motor);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}