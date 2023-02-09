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

    private:
        //Additional Functions
        BDD_ID calcTau();
        BDD_ID characteristicFunction(const std::vector<bool> &stateVector);
        BDD_ID exist_quant(BDD_ID f, BDD_ID x);
        BDD_ID uni_quant(BDD_ID f, BDD_ID x);
        BDD_ID img_not_s_func(BDD_ID f);
        BDD_ID img_s_func(BDD_ID f);

        //Variables
        std::vector<bool>init_state;
        std::vector<BDD_ID>state_bits;
        std::vector<BDD_ID>next_state_bits;
        std::vector<BDD_ID>trans_func;
    /*
    private:
        std::vector<BDD_ID> state_variables;
        std::vector<BDD_ID> next_state_variables;
        std::vector<BDD_ID> transition_functions;
        std::vector<bool> init_state;

        //Additional Functions
        BDD_ID translation_relation();
        BDD_ID bool_to_BDD_ID(const std::vector<bool> &stateVector);
        BDD_ID existential_quantification(BDD_ID f, BDD_ID x);
        BDD_ID img(BDD_ID f);
        BDD_ID img_nxt(BDD_ID f);
    */
    };

}
#endif
