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




















#endif
