#include "Manager.h"
#include <vector>
#include <string>
#include <iostream>
#include <Manager.h>

using namespace ClassProject;

const BDD_ID TrueID = 1;
const BDD_ID FalseID = 0;
//Creates
BDD_ID Manager::createVar(const std::string &label)
{
    //to create a variable we need to create an node. Thats why we return createNode;
    //we need to define createNode
    return createNode(label,BDDTableSize(),False(),True(),BDDTableSize());
}

const BDD_ID &True()
{
    return TrueID;
}

const BDD_ID &False()
{
    return FalseID;
}

bool Manager::isConstant(BDD_ID f)
{
    return f==False() || f==True();
}

bool Manager::isVariable(BDD_ID x)
{
    return x!=False() && x!=True() && topVar(x)==x;
}

BDD_ID Manager::topVar(BDD_ID f)
{
    return BDDTable[f].TopVar_Entry;
}

//The ite-operation is describeded on 2-17
/* if (terminalcase), terminal cases are described on 2.15:
 * ite(1,f,g) translates to: if i= truenode, than return f, otherwise g, but otherwise will NEVER accure
 * ite(0,g,f) translates to: if i=falsenode, than return g, oterwise f, but here otherwise will accure EVERY time
 * ite(f,1,0) translates to: if t=truenode and e=falsenode, return f
 * ite(g,f,f) translates to: if t==e, return e;
 *ite(f,0,1)
 */
BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{
    //Pseudocode see script
    if (i==True()){
        return t;}
    else if (i==False()){
        return e;}
    else if (t==True() && e==False()){
        return i;}
    else if (t==e){
        return e;}
    //not sure about the last terminal case
    /*
    else if (t==False() && e=True()){
        return ;}
    */
    //we use the function Compare to computed BDD to check if the computed table has entry with the same IDs.
    if (auto comp=CompareToComputedBDD(i,t,e);comp!=-1){return comp;}

    BDD_ID ID_of_TopVar_temp;
    //checking for the smallest id. STore the winning id to a temporary variable, this
    if (i <=t)
    {
        if(i<=e)
        {
            ID_of_TopVar_temp=i;
        }
        else if (e<=t)
        {
            ID_of_TopVar_temp=e;
        }
        else
        {
            ID_of_TopVar_temp=t;
        }
    }

    //for rHigh we need cofactor true for i,t,e
    BDD_ID i_co_true= coFactorTrue(i,ID_of_TopVar_temp);
    BDD_ID t_co_true= coFactorTrue(t,ID_of_TopVar_temp);
    BDD_ID e_co_true= coFactorTrue(e,ID_of_TopVar_temp);

    //same for rLow, but cofactor false
    BDD_ID i_co_false= coFactorFalse(i,ID_of_TopVar_temp);
    BDD_ID t_co_false= coFactorFalse(t,ID_of_TopVar_temp);
    BDD_ID e_co_false= coFactorFalse(e,ID_of_TopVar_temp);

    BDD_ID rHigh = ite(i_co_true,t_co_true,e_co_true);
    BDD_ID rLow = ite(i_co_false,t_co_false,e_co_false);

    if (rHigh==rLow){
        return rHigh;
    }
    // the functions used in the next 2 lines are not declared yet,
    BDD_ID r=find_or_add_unique_table();
    void update_computed_table();
    return r;
}
//Pseudo-Code was given in the documentation
BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    if (isConstant(f)){return f;}
    if (topVar(f)==x) {return GetHigh(f);}
    else
    {
        BDD_ID T = coFactorTrue(GetHigh(f),x);
        BDD_ID F = coFactorTrue(GetLow(f),x);
        return ite(topVar(f),T,F);
    }
}
//Pseudo-Code was given in the documentation
BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    if (isConstant(f)){return f;}
    if (topVar(f)==x) {return GetLow(f);}
    else
    {
        BDD_ID T = coFactorFalse(GetHigh(f),x);
        BDD_ID F = coFactorFalse(GetLow(f),x);
        return ite(topVar(f),T,F);
    }
}
//The following 2 functions describe behaviour if x isn't described.
BDD_ID Manager::coFactorTrue(BDD_ID f)
{
    return coFactorTrue(f, topVar(f));
}

BDD_ID Manager::coFactorFalse(BDD_ID f)
{
   return coFactorFalse(f, topVar(f));
}
// Negation is given on slide 2-15 not(f)=ite(f,0,1)
BDD_ID Manager::neg(BDD_ID a)
{
    return ite(a,False(),True());
}
//and given on slide 2-15 f and g = ite(f,g,0)
BDD_ID Manager::and2(BDD_ID a, BDD_ID b)
{
    return ite(a,b,False());
}
//or given on slide 2-15 f or g = ite(f,1,g)
BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
{
    return ite(a,True(),b);
}
//xor given on slide 2-15 f xor g = ite(f,neg(g),g)
BDD_ID Manager::xor2(BDD_ID a, BDD_ID b)
{
    return ite(a,neg(b),b);
}
//same logic like and, f nand g =ite(f,neg(g),1)
//if f is true, output not(g), else 1
BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
{
    return ite(a,neg(b),1);
}
//same logic again. f nor g =ite(neg(f),neg(g),0)
//if neg(f) is true , output neg(g), else 0
BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{
    return ite(neg(a),neg(b),False());
}
//same logic again...
BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{
    return ite(a,b,neg(b))
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    return BDDTable[root].Varname_Entry;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{

}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{

}

size_t Manager::BDDTableSize()
{
    return BDDTable.size();
}

BDD_ID Manager::createNode(std::string NodeName, BDD_ID NodeID, BDD_ID NoteLow, BDD_ID NoteHigh, BDD_ID NoteTop)
{
    // add element to the end of BDDTable, we use class constructor for that
    BDDTable.push_back(BDDEntry(NodeName, NodeID, NoteLow, NoteHigh, NoteTop));
    //ID is always position -1
    return BDDTableSize() -1;
}

BDD_ID Manager::CompareToComputedBDD(BDD_ID x, BDD_ID y, BDD_ID z)
{
    for (auto item : BDDTable_comp)
        if (item.TopVar_Entry==x && item.Low_Entry ==y && item.High_Entry ==z) {return item.BDD_ID_Entry;}
    return -1;
}

BDD_ID Manager::GetHigh(BDD_ID ID)
{
    return BDDTable[ID].High_Entry;
}

BDD_ID Manager::GetLow(BDD_ID ID)
{
    return BDDTable[ID].Low_Entry;
}

BDD_ID find_or_add_unique_table(BDD_ID x,BDD_ID rLow,BDD_ID rHigh)
{

}
void update_computed_table(f,g,h,r)
{

}

//Done at first call of Manager .
Manager::Manager(void)
{
    FalseID = createNode("FALSE",0,0,0,0);
    TrueID = createNode( "TRUE",1,1,1,1);
}