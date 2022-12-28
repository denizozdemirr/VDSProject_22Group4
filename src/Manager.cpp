#include "Manager.h"
#include <vector>
#include <string>
#include <iostream>
#include <Manager.h>

using namespace ClassProject;

//Done at first call of Manager .
Manager::Manager(void)
    {
    BDD_ID trash;
    trash=createNode("FALSE",0,0,0,0);
    trash=createNode( "TRUE",1,1,1,1);
}

//Manager::~Manager(){}

//Creates
BDD_ID Manager::createVar(const std::string &label)
{
    //to create a variable we need to create an node. Thats why we return createNode;
    //we need to define createNode
    return createNode(label,BDDTableSize(),False(),True(),BDDTableSize());
}

const BDD_ID &Manager::True()
{
    return TrueID;
}

const BDD_ID &Manager::False()
{
    return FalseID;
}

bool Manager::isConstant(BDD_ID f)
{
    if (f==False() || f==True())
    {
        return True();
    }
    return False();
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
    //Pseudocode see script 2-17
    //if terminal case
    if (i==True()){
        return t;}
    else if (i==False()){
        return e;}
    else if (t==True() && e==False()){
        return i;}
    else if (t==e){
        return e;}
    //not sure about the last terminal case, so far we have an neg, so I pretty sure this case is not needed
    /*
    else if (t==False() && e=True()){
        return ;}
    */
    //we use the function Compare to computed BDD to check if the computed table has entry with the same IDs.
    //computed table has entry i, t or e
    for (int i = 0; i < COMPTable.size(); i++)
    {
        if (COMPTable[i].f == i && COMPTable[i].g == t && COMPTable[i].h == e)
        {
            return COMPTable[i].r;
        }

    }
    //Get the smallest value of topvariable from entry i, t and e
    //BDD_ID ID_of_TopVar_temp=GetMinTop(i, t, e);

    //set top of i as first possible candidate for being the smallest number
    BDD_ID ID_of_TopVar_temp=topVar(i);
    //test if the topVar of t is not a constant (True/False) nad if t is smaller than the one of i
    if(!isConstant(t) && topVar(t)<ID_of_TopVar_temp)
    {
        ID_of_TopVar_temp=topVar(t);
    }
    if(!isConstant(e) && topVar(e)<ID_of_TopVar_temp)
    {
        ID_of_TopVar_temp=topVar(e);
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

    if (rHigh==rLow)
    {
        BDD_ID rReturn =rHigh;
        return rHigh;//return rHigh;
    }
    BDD_ID r = find_or_add_unique_table(ID_of_TopVar_temp,rLow,rHigh);

    update_computed_table(i, t, e,r);

    return r;
}
//Pseudo-Code was given in the documentation
BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    //Check if f is a constant (true or false)
    if (isConstant(f))
    {
        return f;
    }
    if (topVar(f)==x)
    {
        return GetHigh(f);
    }
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
    if (isConstant(f))
    {
        return f;
    }
    if (topVar(f)==x)
    {
        return GetLow(f);
    }
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
    return ite(a,b,neg(b));
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    return BDDTable[root].Varname_Entry;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    //root itself is always reachable
    nodes_of_root.insert(root);
    if (isConstant(root)){return;}
    else {
        //We call findNodes again with high and low of variable, we use them as new root.
        findNodes(BDDTable[root].High_Entry, nodes_of_root);
        findNodes(BDDTable[root].Low_Entry, nodes_of_root);
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{
    if (isConstant(root)){return;}
    else {
        //if root is no leaf node insert the TopVar of root as first vars of root.
        vars_of_root.insert(BDDTable[root].TopVar_Entry);
        //We call findVars again with high and low of variable, we use them as new root.
        findVars(BDDTable[root].High_Entry, vars_of_root);
        findVars(BDDTable[root].Low_Entry, vars_of_root);
    }
}

size_t Manager::BDDTableSize()
{
    return BDDTable.size();
}

BDD_ID Manager::createNode(std::string NodeName, BDD_ID NodeID, BDD_ID NoteLow, BDD_ID NoteHigh, BDD_ID NoteTop)
{
    // add element to the end of BDDTable, we use class constructor for that
    BDDTable.push_back(BDDEntry(NodeName, NodeID, NoteHigh, NoteLow, NoteTop));
    //ID is always position -1
    return BDDTableSize() -1;
}

// Computed Table has Entry for (f,g,h)
/*bool Manager::CompareToComputedBDD(BDD_ID f, BDD_ID g, BDD_ID h)
{
    for (int i = 0; i < COMPTable.size(); i++)
    {
        if (COMPTable[i].f == f && COMPTable[i].g == g && COMPTable[i].h == h)
        {
            return COMPTable[i].r;
        }

    }
    return 2;
    /*
    for (auto item : COMPTable)
        if (item.f==f && item.g ==g && item.h ==h)
        {
            return item.r;
        }
        return -1;

}
*/
BDD_ID Manager::GetHigh(BDD_ID ID)
{
    return BDDTable[ID].High_Entry;
}

BDD_ID Manager::GetLow(BDD_ID ID)
{
    return BDDTable[ID].Low_Entry;
}

BDD_ID Manager::find_or_add_unique_table(BDD_ID x,BDD_ID rLow,BDD_ID rHigh)
{
    //for loop to check every element of the unique table
    for (BDD_ID count=0; count<=BDDTableSize(); count++)
    {
        if (BDDTable[count].TopVar_Entry==x && BDDTable[count].Low_Entry==rLow && BDDTable[count].High_Entry==rHigh)
        {
            return count;
        }
        else if(count == BDDTableSize())
        {

            return createNode("",count,rLow,rHigh,x);
        }
    }
}
void Manager::update_computed_table(BDD_ID f,BDD_ID g,BDD_ID h, BDD_ID r)
{
    COMPTable.push_back(ComputedEntry(f,g,h,r));
}
/*
BDD_ID Manager::GetMinTop(BDD_ID x, BDD_ID y, BDD_ID z)
{
    BDD_ID TopVar1 = topVar(x);
    BDD_ID TopVar2 = topVar(y);
    BDD_ID TopVar3 = topVar(z);
    if (TopVar1<=TopVar2)
    {
        if (TopVar1<=TopVar3)
        {
           return TopVar1;
        }
        else
        {
            return TopVar3;
        }
    }
    else
    {
        if (TopVar2<=TopVar3)
        {
            return TopVar2;
        }
        else
        {
            return TopVar3;
        }
    }
}
*/