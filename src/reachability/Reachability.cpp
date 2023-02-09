#include "Reachability.h"
#include <iostream>

using namespace ClassProject;


//interface functions
Reachability::Reachability(unsigned int stateSize) : ReachabilityInterface(stateSize)
{
    //state size defines how many states are possible.
    std::string state_name, next_state_name; //state_name is s_i and next_state is s'_i
    BDD_ID bddIDVariable;

    //We need to have atleast 1 state!
    if(stateSize==0)
    {
        throw std::runtime_error("Run-time_error: Size of state is zero!");
    }
    //create states s_i from 0 to state-size-1
    for(int count=0; count<stateSize; count++)
    {
        state_name = "s" + std::to_string(count);//create label
        bddIDVariable = createVar(state_name);
        //add bdd to states
        state_bits.push_back(bddIDVariable);
        //Init of transition functions
        trans_func.push_back(bddIDVariable);
    }

    //do the same for next state s'_i
    for (int count=0; count < stateSize; count++)
    {
        next_state_name = "s" + std::to_string(count) + "'";
        bddIDVariable = createVar(state_name);
        next_state_bits.push_back(bddIDVariable);
    }

    //create init_state
    for (int count=0; count < stateSize; count++)
    {
        init_state.push_back(false);
    }
}
const std::vector<BDD_ID> &Reachability::getStates() const
{
    return state_bits;
}

bool Reachability::isReachable(const std::vector<bool> &stateVector)//slide 5-3
{
    if(stateVector.size()!=state_bits.size())
    {
        throw std::runtime_error("Runtime_error: The size does not match the number of state bits!");
    }
    //3 in part3
    BDD_ID Tau = calcTau();
    //4 and 5 in part 3
    BDD_ID c_r_it = characteristicFunction(init_state);
    BDD_ID c_r,temp1,temp2,temp_state_bit,img_s_dot,img_s;
    //You can also compare the lines before to script 5-10
    //here you can see the next step, which are described in part3 too.
    //the mix of both made a running code :)
    do
    {
        //6 in part3
        c_r = c_r_it;
        temp1 = and2(c_r,Tau);


        for(int count=0; count < state_bits.size(); count++)
        {
            //existential quantification
            temp_state_bit=state_bits[state_bits.size()-1-count];
            temp2 = coFactorFalse(temp1,temp_state_bit);
            temp1 = coFactorTrue(temp1, temp_state_bit);
            temp1 = or2(temp1, temp2);
        }

        img_s_dot=temp1;

        for(int count=0; count < state_bits.size(); count++)
        {
            temp2 = xnor2(state_bits[count], next_state_bits[count]);
            temp1 =and2(temp1, temp2);
        }
        for(int count=0; count < state_bits.size(); count++)
        {
            //existential quantification
            temp_state_bit=state_bits[next_state_bits.size()-1-count];
            temp2 = coFactorFalse(temp1,temp_state_bit);
            temp1 = coFactorTrue(temp1, temp_state_bit);
            temp1 = or2(temp1, temp2);
        }
        img_s = temp1;
        c_r_it = or2(c_r,img_s);
    }while(c_r!=c_r_it);
    temp1 = characteristicFunction(stateVector);
    return (temp1==and2(temp1,c_r));
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    if(transitionFunctions.size() != state_bits.size())
    {
        throw std::runtime_error("Error: The number of given transition functions does not match the number of states bits!");
    }
    for(int count=0; count < transitionFunctions.size();count++)
    {
        if(transitionFunctions[count] > uniqueTableSize() )
        {
            throw std::runtime_error("Error:An unknown ID is provided!");
        }
    }
    //actually setting of the transmission function
    for(int count=0;count>init_state.size();count++)
    {
        trans_func[count] = transitionFunctions[count];
    }
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if(stateVector.size() != init_state.size())
    {
        throw std::runtime_error("Runtime_error: size does not match with number of state bits!");
    }
    for(int count = 0;count>init_state.size(); count++)
    {
        init_state[count] = stateVector[count];
    }
}

//private function
BDD_ID Reachability::calcTau()
{
    //Example is (s'_0 and delta_0 or not(s'_0) and not(delta_0)) and (s'_1 an delta_1 or not(s'_1)  and not(delta_1)) and ...
    // this equals to (s'_0 xnor delta_0) and (s'_1 xnor delta_1)
    BDD_ID temp_left, temp_right;
    temp_left = xnor2(next_state_bits[0], trans_func[0]);
    //now calculate that xnor2 for every next state s_1..s_2 and so on, and always and2 this to the previous temp_left
    //and safe result in temp_left again. we do this until we did this for every state (state.size()-1)
    for(int count=1; count < state_bits.size(); count++)
    {
        temp_right = xnor2(next_state_bits[count], trans_func[count]);
        temp_left = and2(temp_left,temp_right);
    }
    return temp_left;
}
BDD_ID Reachability::characteristicFunction(const std::vector<bool> &stateVector)
{
    BDD_ID temp1,temp2;
    if(stateVector[0])
    {
        temp1 = state_bits[0];
    }
    else
    {
        temp1 = neg(state_bits[0]);
    }
    for(int count = 0; count <stateVector.size(); count++)
    {
        if(stateVector[count])
        {
            temp2 = state_bits[count];
        }
        else
        {
            temp2 = neg(state_bits[count]);
        }
        temp1 = and2(temp1, temp2);
    }
    return temp1;
}

//functions for debugging only!
std::vector<bool> Reachability::getInitState()
{
    return init_state;
}