#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"

namespace ClassProject
{

    class Reachability : public ReachabilityInterface
    {
    //Functions for the Interface
    public:
        Reachability(unsigned int stateSize);//DONE
        //named like the function given in the ReachabilityInterface
        const std::vector<BDD_ID> &getStates() const override;
        bool isReachable(const std::vector<bool> &stateVector) override; //slide 5-3
        void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;//DONE
        void setInitState(const std::vector<bool> &stateVector) override;
        std::vector<bool> getInitState();

    private:
        //Additional Functions
        BDD_ID calcTau();
        BDD_ID characteristicFunction(const std::vector<bool> &stateVector);

        //Variables
        std::vector<bool>init_state;
        std::vector<BDD_ID>state_bits;
        std::vector<BDD_ID>next_state_bits;
        std::vector<BDD_ID>trans_func;
    };

}
#endif
