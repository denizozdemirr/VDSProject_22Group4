#include "Reachability.h"
#include <iostream>

using namespace ClassProject;


//interface functions ,state size defines how many states are possible.
Reachability::Reachability(unsigned int stateSize) : ReachabilityInterface(stateSize)
{
    std::string state_name, next_state_name; //state_name is s_i and next_state is s'_i
    BDD_ID bddIDVariable;

    if(stateSize==0)//We need to have atleast 1 state!
        throw std::runtime_error("Run-time_error: Size of state is zero!");
    for(int count=0; count<stateSize; count++)//create states s_i from 0 to state-size-1
    {
        state_name = "s" + std::to_string(count);//create label
        bddIDVariable = createVar(state_name);
        //add bdd to states
        state_bits.push_back(bddIDVariable);
        //Init of transition functions
        trans_func.push_back(bddIDVariable);
        init_state.push_back(false);
        next_state_name = "s" + std::to_string(count) + "'";
        bddIDVariable = createVar(state_name);
        next_state_bits.push_back(bddIDVariable);
    }
}

const std::vector<BDD_ID> &Reachability::getStates() const
{
    return state_bits;
}

bool Reachability::isReachable(const std::vector<bool> &stateVector)//slide 5-3
{
    if(stateVector.size()!=state_bits.size())
        throw std::runtime_error("Runtime_error: The size does not match the number of state bits!");
    //3 in part3
    BDD_ID Tau = calcTau();// correct
    //4 and 5 in part 3
    BDD_ID c_r_it = characteristicFunction();//correct
    BDD_ID c_r,temp_ir,img_s_dot,img_s,temp1,temp2;
    //You can also compare the lines before to script 5-10
    //here you can see the next step, which are described in part3 too.
    //the mix of both made a running code :)
    do
    {
        c_r = c_r_it;//6 in part 3
        temp_ir = and2(c_r,Tau);//7 in part 3 //correct
        img_s_dot= img_not_s_func(temp_ir);//7 in part 3 //correct
        img_s    = img_s_func(img_s_dot);//8 in part 3 //correct
        c_r_it = or2(c_r,img_s);// 9 in part 3 //correct
    }while(c_r!=c_r_it);//10 in part3 fixed point reached
    for(int count = 0; count < state_bits.size(); count++)//11 in part3
    {
        if(stateVector[count])
            c_r_it = coFactorTrue(c_r_it,state_bits[count]);
        else
            c_r_it = coFactorFalse(c_r_it,state_bits[count]);
    }
    return (c_r_it);
}

void Reachability::setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions)
{
    if(transitionFunctions.size() != state_bits.size())
        throw std::runtime_error("Error: The number of given transition functions does not match the number of states bits!");

    for(int count=0; count < transitionFunctions.size();count++)
    {
        if(transitionFunctions[count] >= uniqueTableSize() )
            throw std::runtime_error("Error:An unknown ID is provided!");
    }
    //actually setting of the transmission function
    trans_func = transitionFunctions;
}

void Reachability::setInitState(const std::vector<bool> &stateVector)
{
    if(stateVector.size() != init_state.size())
        throw std::runtime_error("Runtime_error: size does not match with number of state bits!");
    init_state = stateVector;
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
BDD_ID Reachability::characteristicFunction()
{
    BDD_ID temp1,temp2;
    //bool to bdd_id
    temp2 = xnor2(state_bits[0],init_state[0]);
    for(int count = 1; count <init_state.size();count++)
        temp2 = and2(temp2,xnor2(state_bits[count],init_state[count]));
    return temp2;
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
    for(int count = state_bits.size()-1; count >= 0; count--)
    {
        temp1 = exist_quant(temp1, state_bits[count]);
    }
    return temp1;
}

BDD_ID Reachability::img_s_func(BDD_ID f)
{
    BDD_ID temp1,temp2;
    temp1=f;
    //cs is calculated with characteristicFunction()
    for(int count = 0; count < state_bits.size() ; count++)
    {
        temp2 = xnor2(state_bits[count], next_state_bits[count]);
        temp1 =and2(temp1, temp2);
    }
    for(int count=state_bits.size()-1; count >= 0 ; count--)
    {
        temp1 = exist_quant(temp1, next_state_bits[count]);
    }
    return temp1;
}