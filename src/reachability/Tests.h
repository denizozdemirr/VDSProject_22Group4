//
// Created by ludwig on 22.11.16.
//
//Changed by schwarz on 08.02.23

#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test {
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm4 = std::make_unique<ClassProject::Reachability>(4);
    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> stateVars4 = fsm4->getStates();
    std::vector<BDD_ID> transitionFunctions;
};

//Check if the reachability constructor throws an runtime error if input size is 0.
TEST_F(ReachabilityTest, ReachabilityConstructorError)
{
    EXPECT_THROW({ClassProject::Reachability Test_Reachability(0);} , std::runtime_error);
}

//Check if functions SetInitState throws a runtime error if the stateVector is bigger than the size we set before.
//
TEST_F(ReachabilityTest, SetInitState)
{
    std::vector<bool> TestInitState = {false,true};
    EXPECT_THROW( {fsm2->setInitState({true, false, false});}, std::runtime_error);
    EXPECT_THROW( {fsm2->setInitState({false});}, std::runtime_error);
    EXPECT_NO_THROW( {fsm2->setInitState({false,false});});
    EXPECT_NO_THROW( {fsm2->setInitState({false,true});});
}

TEST_F(ReachabilityTest, isReachableNoTrans) {
    /*
     * for detailed description see test isReachableFunctionalityTest
     * by default this functionality uses the identiy
     * {s0';s1'}={s_0;s_1}
     *  So the same state is always reachable again, but any other state isnt reachable.
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     *
     */
    fsm2->setInitState({false,false});//State A
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({true,false});//State B
    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({false,true});//State C
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_TRUE(fsm2->isReachable({false, true}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({true,true});//State D
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));
}


TEST_F(ReachabilityTest, getStates) {
    std::vector<BDD_ID> States_Expected = {2, 3};
    EXPECT_EQ(States_Expected, fsm2->getStates());
}


TEST_F(ReachabilityTest, isReachableError) {
    EXPECT_THROW( {fsm2->isReachable({true, false, false});}, std::runtime_error);
    EXPECT_NO_THROW( fsm2->isReachable({true, false}););
}

//originally given

TEST_F(ReachabilityTest, isReachableFunctionalityTest) { // NOLINT


    /* We have a 2^2 space of possible states :=4
     * let_s name this states in alphabetical order : a,b,c,d
     * each state is represented by it's corresponding bits
     * general state={state_bit0;state_bit1}
     * -> A={false,false}; B={true,false}; C={false,true}; D={true,true}
     * the transition is described by {state_bit0';state_bit1'}={bool_function(s_0;s_1);bool_function(s_0;s_1)}
     * let's make an example function: {state_bit0';state_bit1'}={neg(state_bit0);and2(state_bit0;state_bit1)}
     * Now let's perform this on any inital state that is possible.
     *
     ***Initial State A
     * {false,false} (keep  state_bit0';state_bit1'} in mind)
     * ->{neg(false);and2(false;false)} ->{true,false}
     * Next State is B
     * ->{neg(true);and2(true;false)} ->{false,false}
     * Next State is A
     * ->{neg(false);and2(false;false)} ->{true,false}
     * Next State is B
     * !! ->A->B->A->B->...
     ***Initial State B
     * {true,false}
     * ->{neg(true);and2(true;false)} ->{false,false}
     * Next State is A
     * !! There we Go again ->B->A->B->A....
      ***Initial State C
      * {false,true}
      * ->{neg(false);and2(false;true)} ->{true,false}
      * Next State is B
      * ->{neg(true);and2(true;false)} ->{false,false}
      * Next State is A
      * ->{neg(false);and2(false;false)} ->{true,false}
      * !!!There we go again ->C->B->A->B->...
      ***Initial State D
      * {true,true}
      * ->{neg(true);and2(true;true)} ->{false,true}
      * Next State is B
      * ->{neg(false);and2(false;true)} ->{true,false}
      * Next State is A
      * !!!->D->B->A->B->...
      *
      * !!!!!!!!!!!!!!!!!!!!!!!!!CONCLUSION!!!!!!!!!!!!!!!!!!!!!!!!!
      * A and B are always reachable, C and D are never reachable.
      * A={false,false}; B={true,false}; C={false,true}; D={true,true}
      *
     */
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->and2(s0,s1)); // s1' = and(s0;s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});//State A
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({true,false});//State B
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({false,true});//State C
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));

    fsm2->setInitState({true,true});//State D
    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_TRUE(fsm2->isReachable({true, false}));
    EXPECT_FALSE(fsm2->isReachable({true, true}));
}
#endif