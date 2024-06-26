#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>

#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>

#include "customBlocks/customController.hpp"

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    PeripheralInput<> encoder;
    CustomController<> controller;
    PeripheralOutput<> motor;

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP