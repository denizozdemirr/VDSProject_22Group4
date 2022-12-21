//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"
using ClassProject::BDD_ID;

class ManagerTests : public ::testing::Test{
    public:
        ClassProject::Manager manager;
};

TEST_F(ManagerTests,CHECKcreateVarSize)
{
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    manager.createVar("d");
    EXPECT_EQ(manager.BDDTableSize(),6);
}

TEST_F(ManagerTests,CHECKcreateVarSetValues)
{
    EXPECT_EQ(manager.topVar(2),2);
    EXPECT_EQ(manager.topVar(3),3);
    EXPECT_EQ(manager.topVar(4),4);
    EXPECT_EQ(manager.topVar(5),5);

    EXPECT_EQ(manager.GetHigh(2),1);
    EXPECT_EQ(manager.GetHigh(3),1);
    EXPECT_EQ(manager.GetHigh(4),1);
    EXPECT_EQ(manager.GetHigh(5),1);

    EXPECT_EQ(manager.GetLow(2),0);
    EXPECT_EQ(manager.GetLow(3),0);
    EXPECT_EQ(manager.GetLow(4),0);
    EXPECT_EQ(manager.GetLow(5),0);
}

//Check function True
TEST_F(ManagerTests,CHECKTrue)
{
    EXPECT_EQ(manager.True(),1);
}

//Check function False
TEST_F(ManagerTests,CHECKFalse)
{
    EXPECT_EQ(manager.False(),0);
}

//Check function isConstant
TEST_F(ManagerTests,CHECKisConstant)
{
    EXPECT_EQ(manager.isConstant(0),1);
    EXPECT_EQ(manager.isConstant(1),1);
    EXPECT_EQ(manager.isConstant(2),0);
    EXPECT_EQ(manager.isConstant(3),0);
    EXPECT_EQ(manager.isConstant(4),0);
    EXPECT_EQ(manager.isConstant(5),0);
}

//checking Function topVar is done within checking the creation of Variables. If Something fails there, we can check furthermore.
//Check function ite
TEST_F(ManagerTests,CHECKite)
{
    //Check for terminal cases
    EXPECT_EQ(manager.ite(4,1,0),4);
    EXPECT_EQ(manager.ite(5,2,2),2);
    EXPECT_EQ(manager.ite(0,3,5),5);
    EXPECT_EQ(manager.ite(1,3,5),3);

    //Check General Cases of ite
    EXPECT_EQ(manager.ite(4,3,5),6);
    EXPECT_EQ(manager.ite(2,3,4),7);
}
//Check neg function
TEST_F(ManagerTests,CHECKneg)
{
    EXPECT_EQ(manager.neg(3),8);
}

//Check and2 function
TEST_F(ManagerTests,CHECKand2)
{
    EXPECT_EQ(manager.and2(8,6),9);
}

//Check or2 function
TEST_F(ManagerTests,CHECKor2)
{
    EXPECT_EQ(manager.or2(8,9),10);
}

//Check xor2 function
TEST_F(ManagerTests,CHECKxor2)
{
    EXPECT_EQ(manager.xor2(2,5),11);
}

//Check nand2 function
TEST_F(ManagerTests,CHECKnand2)
{
    EXPECT_EQ(manager.nand2(11,4),12);
}

//Check nor2 function
TEST_F(ManagerTests,CHECKnor2)
{
    EXPECT_EQ(manager.nor2(3,4),13);
}

//Check xnor2 function
TEST_F(ManagerTests,CHECKxnor2)
{
    EXPECT_EQ(manager.xnor2(13,2),14);
}

//I have no idea how to implement them
//Check the CoFactorTrue
/*
TEST_F(ManagerTests,CHECKcoFactorTrue)
{
    EXPECT_EQ(manager.coFactorTrue(,),);
}

//Check the CoFactorFalse
TEST_F(ManagerTests,CHECKcoFactorFalse)
{
    EXPECT_EQ(manager.coFactorTrue(,),);
}

//Check the CoFactorTrue only with one input

TEST_F(ManagerTests,CHECKcoFactorTrue)
{
    EXPECT_EQ(manager.coFactorTrue(),);
}

//Check the CoFactorTrue only with one input

TEST_F(ManagerTests,CHECKcoFactorFalse)
{
    EXPECT_EQ(manager.coFactorTrue(),);
}


//Check function sample-build
TEST_F(ManagerTests,CHECKsample-build)
{
    EXPECT_EQ(manager.);
}
*/
















#endif
