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
    BDD_ID c_r,temp_ir,img_s_dot,img_s;
    //You can also compare the lines before to script 5-10
    //here you can see the next step, which are described in part3 too.
    //the mix of both made a running code :)
    do
    {
        c_r = c_r_it;
        temp_ir = and2(c_r,Tau);
        img_s_dot= img_not_s_func(temp_ir);
        img_s    = img_s_func(img_s_dot);
        c_r_it = or2(c_r,img_s);
    }while(c_r!=c_r_it);
    temp_ir = characteristicFunction(stateVector);
    return (temp_ir == and2(temp_ir,c_r));
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
    for(int count=0;count< transitionFunctions.size();count++)
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
    BDD_ID temp1, temp2;
    temp1 = xnor2(next_state_bits[0], trans_func[0]);
    //now calculate that xnor2 for every next state s_1..s_2 and so on, and always and2 this to the previous temp_left
    //and safe result in temp_left again. we do this until we did this for every state (state.size()-1)
    for(int count=1; count < state_bits.size(); count++)
    {
        temp2 = xnor2(next_state_bits[count], trans_func[count]);
        temp1 = and2(temp1,temp2);
    }
    return temp1;
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

    for(int count = 1; count <stateVector.size(); count++)
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

BDD_ID Reachability::exist_quant(BDD_ID f, BDD_ID x)
{
    //existential quantification
    BDD_ID temp1,temp2;
    temp1 = coFactorTrue(f, x);
    temp2 = coFactorFalse(f, x);
    return or2(temp1, temp2);
}

BDD_ID Reachability::img_not_s_func(BDD_ID f)
{
    BDD_ID temp1=f;
    for(int count = 0; count < state_bits.size(); count++)
    {
        temp1 = exist_quant(temp1, state_bits[state_bits.size()-1-count]);
    }
    return temp1;
}

BDD_ID Reachability::img_s_func(BDD_ID f)
{
    BDD_ID temp1,temp2;
    temp1=f;
    //cs is calculated with characteristicFunction()
    for(int count=0; count < state_bits.size(); count++)
    {
        temp2 = xnor2(state_bits[count], next_state_bits[count]);
        temp1 =and2(temp1, temp2);
    }
    //temp1=and2(f,characteristicFunction(init_state));

    for(int count=0; count < state_bits.size(); count++)
    {
        temp1 = exist_quant(temp1, next_state_bits[state_bits.size()-1-count]);
    }
    return temp1;
}