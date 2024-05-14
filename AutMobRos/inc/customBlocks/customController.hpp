#ifndef CUSTOMBLOCKTEMPLATE_HPP_
#define CUSTOMBLOCKTEMPLATE_HPP_

#include <eeros/control/Blockio.hpp>
// Include header files for the subblocks

using namespace eeros::control;

template <typename T = double>
class CustomController : public Blockio<1,1,T>   // (!)Set the number of inputs and outputs
{
public:
    CustomController() : phi_S(0.0), kp(2.0), U(0.0)
    {
        // Connect subblocks, initialize variables, ...
    }

    // Implement getter functions for the subsystem inputs
    void setPhi_S(double setPointValue){
        this->phi_S = setPointValue;
    }


    virtual void run()
    {
        // dummy code feeding input straight to output
        U = kp*(phi_S-this->in.getSignal().getValue());
        
        // Ouput
        this->out.getSignal().setValue(U); // Value
        this->out.getSignal().setTimestamp(this->in.getSignal().getTimestamp());

    }

protected:
    // Define intermediate variables and subblocks
    double phi_S, kp, U;
};

#endif //CUSTOMBLOCKTEMPLATE_HPP_
