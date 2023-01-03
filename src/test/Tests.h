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
//Create variables and check if their set LowSuccesor, HighSuccesor and TopVariable is correct.
TEST_F(ManagerTests,CHECKcreateVarSetValues)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
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

//Check if function true returns id 1
TEST_F(ManagerTests,CHECKTrue)
{
    EXPECT_EQ(manager.True(),1);
}

//Check function False returns id 0
TEST_F(ManagerTests,CHECKFalse)
{
    EXPECT_EQ(manager.False(),0);
}

//Check function isConstant, we check if created variables are detected as non constant.
TEST_F(ManagerTests,CHECKisConstant)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    EXPECT_EQ(manager.isConstant(0),1);
    EXPECT_EQ(manager.isConstant(1),1);
    EXPECT_EQ(manager.isConstant(2),0);
    EXPECT_EQ(manager.isConstant(3),0);
    EXPECT_EQ(manager.isConstant(4),0);
    EXPECT_EQ(manager.isConstant(5),0);
}

//Check function find_or_add_unique_table
TEST_F(ManagerTests,CHECKfindoradduniquetable)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    EXPECT_EQ(manager.find_or_add_unique_table(6,0,1),6);
}

//checking Function topVar is done within checking the creation of Variables. If Something fails there, we can check furthermore.
//Check function ite terminal cases.

TEST_F(ManagerTests,CHECKite)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    //Check for terminal cases
    EXPECT_EQ(manager.ite(4,1,0),4);
    EXPECT_EQ(manager.ite(5,2,2),2);
    EXPECT_EQ(manager.ite(0,3,5),5);
    EXPECT_EQ(manager.ite(1,3,5),3);
}

//Check neg function
TEST_F(ManagerTests,CHECKneg)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    ClassProject::BDD_ID Aneg = manager.neg(2);
    EXPECT_EQ(manager.GetLow(Aneg),1);
    EXPECT_EQ(manager.GetHigh(Aneg),0);

}

//Check and2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKand2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    ClassProject::BDD_ID AandB = manager.and2(2,3);
    EXPECT_EQ(manager.GetLow(AandB),0);
    EXPECT_EQ(manager.GetHigh(AandB),3);
    ClassProject::BDD_ID CandD = manager.and2(4,5);
    EXPECT_EQ(manager.GetLow(CandD),0);
    EXPECT_EQ(manager.GetHigh(CandD),5);
    //EXPECT_EQ(manager.and2(4,5),8);
}

//Check or2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKor2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    ClassProject::BDD_ID AorC = manager.or2(2,4);
    EXPECT_EQ(manager.GetLow(AorC),4);
    EXPECT_EQ(manager.GetHigh(AorC),1);
    ClassProject::BDD_ID BorD = manager.or2(3,5);
    EXPECT_EQ(manager.GetLow(BorD),5);
    EXPECT_EQ(manager.GetHigh(BorD),1);
}

//Check xor2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKxor2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    // ( a AND NOT(d)) OR (NOT(a) AND d)
    ClassProject::BDD_ID AxorD = manager.xor2(2,5);
    EXPECT_EQ(manager.GetLow(AxorD),5);
    EXPECT_EQ(manager.GetHigh(AxorD),manager.neg(5));
    // ( b AND NOT(c)) OR (NOT(b) AND c)
    ClassProject::BDD_ID BxorC = manager.xor2(3,4);
    EXPECT_EQ(manager.GetLow(BxorC),4);
    EXPECT_EQ(manager.GetHigh(BxorC),manager.neg(4));
}

//Check nand2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKnand2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    ClassProject::BDD_ID AnandB = manager.nand2(2,3);
    EXPECT_EQ(manager.GetLow(AnandB),1);
    EXPECT_EQ(manager.GetHigh(AnandB),manager.neg(3));
    ClassProject::BDD_ID CnandD = manager.nand2(4,5);
    EXPECT_EQ(manager.GetLow(CnandD),1);
    EXPECT_EQ(manager.GetHigh(CnandD),manager.neg(5));
}

//Check nor2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKnor2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    ClassProject::BDD_ID AnorC = manager.nor2(2,4);
    EXPECT_EQ(manager.GetLow(AnorC),manager.neg(4));//->if a=0->NOT(c)
    EXPECT_EQ(manager.GetHigh(AnorC),0);//->if a=1->0
    ClassProject::BDD_ID BnorD = manager.nor2(3,5);
    EXPECT_EQ(manager.GetLow(BnorD),manager.neg(5));//->if b=0->NOT(d)
    EXPECT_EQ(manager.GetHigh(BnorD),0);// if b=1->0
}

//Check xnor2 function, we check if the the LowSuccesor and HighSuccesor of the created node correspondends to the expected values
TEST_F(ManagerTests,CHECKxnor2)
{
    EXPECT_EQ(manager.createVar("a"),2);
    EXPECT_EQ(manager.createVar("b"),3);
    EXPECT_EQ(manager.createVar("c"),4);
    EXPECT_EQ(manager.createVar("d"),5);
    EXPECT_EQ(manager.BDDTableSize(),6);
    // ( a AND d) OR (NOT(a) AND NOT(d))
    ClassProject::BDD_ID AxnorD = manager.xnor2(2,5);
    EXPECT_EQ(manager.GetLow(AxnorD),manager.neg(5));
    EXPECT_EQ(manager.GetHigh(AxnorD),5);
    // ( b AND c) OR (NOT(b) AND NOT(c))
    ClassProject::BDD_ID BxnorC = manager.xnor2(3,4);
    EXPECT_EQ(manager.GetLow(BxnorC),manager.neg(4));
    EXPECT_EQ(manager.GetHigh(BxnorC),4);
}
#endif
