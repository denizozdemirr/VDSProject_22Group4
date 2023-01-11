#include "Manager.h"
#include <vector>
#include <string>
#include <iostream>
#include <Manager.h>
#include <unordered_map>

using namespace ClassProject;

//Done at first call of Manager .
Manager::Manager(void)
{
    BDDTable.clear();
    COMPTable.clear();
    createNode("FALSE",0,0,0,0);
    createNode( "TRUE",1,1,1,1);
}

Manager::~Manager(){}

//Creates Variable by calling the createNode function and returns the ID, which is returned by createNode iteself.
BDD_ID Manager::createVar(const std::string &label)
{
    //to create a variable we need to create an node. Thats why we return createNode;
    //we need to define createNode
    return createNode(label,uniqueTableSize(),False(),True(),uniqueTableSize());
}

//Return ID of True Node
const BDD_ID &Manager::True()
{
    return TrueID;
}

//Return ID of False Node
const BDD_ID &Manager::False()
{
       return FalseID;
}

//Return 1 if ID f represents false or true note. Else return 0.
bool Manager::isConstant(BDD_ID f)
{
        return f==False() || f==True();
}

//x is an variable if it's not false or true node and the top variable equals the id, returns 1 if x is an variable, 0 else.
bool Manager::isVariable(BDD_ID x)
{
    return x!=False() && x!=True() && topVar(x)==x;
}

//returns the topVar of ID f
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
    if (i==True())
    {
        return t;
    }
    else if (i==False())
    {
        return e;
    }
    else if (t==True() && e==False())
    {
        return i;
    }
    else if (t==e)
    {
        return e;
    }
    //check if computed table has entry i, t or e
    //we just check for the exact same pattern so far.
    //Maybe we can advance that somehow to also check for patterns not exactly included in the computeted table,
    //but represented by the same boolean function
        //Standard triples

        //ite(F,F,G)->ite(F,1,G)
        if (i == t && i != e)
        {
            t = True();
        }
        //ite(F,G,F)->ite(F,G,0)
        else if (i == e && i != t)
        {
            e = False();
        }
        /*
        //ite(F,G,not(F))->ite(F,G,1)
        else if (i == topVar(e) && i != t)
        {
            e = True();
        }
        //ite(F,not(F),G)->ite(F,0,G)
        else if (i == topVar(t) && i != e) {
            t = False();
        }
         */
    BDD_ID i_temp=i;
    BDD_ID t_temp=t;
    BDD_ID e_temp=e;

    //equivalent pairs:
        //ite(F,1,G)=(G,1,F)
        if (t == True() && topVar(i)> topVar(e))
        {
            i_temp=e;
            t_temp=t;
            e_temp=i;
        }
        else if(t == True() && topVar(i)==topVar(e) && i>e)
        {
            i_temp=e;
            t_temp=t;
            e_temp=i;
        }
        //ite(F,G,0)=(G,F,0)
        else if (e == False() && topVar(i)> topVar(t))
        {
            i_temp=t;
            t_temp=i;
            e_temp=e;
        }
        else if(e == False() && topVar(i)==topVar(t) && i>t)
        {
            i_temp=t;
            t_temp=i;
            e_temp=e;
        }
        /*
        //ite(F,G,1)=ite(neg(G),neg(F),1) G=Top(neg(G) ->ite(neg(G),neg(F),1)=ite(Top(t),Top(i),e)
        else if (e == True() && topVar(topVar(i)) > topVar(topVar(t)))
        {
            i_temp= topVar(t);
            t_temp= topVar(i);
            e_temp=e;
        }
        else if(e == True() && topVar(topVar(i))==topVar(topVar(t)) && topVar(i)>topVar(t))
        {
            i_temp= topVar(t);
            t_temp= topVar(i);
            e_temp=e;
        }

        //ite(F,0,G)=ite(neg(G),0,neg(f))
        else if (t == False() && topVar(topVar(i))>topVar(topVar(e)))
        {
            i_temp= topVar(e);
            t_temp= t;
            e_temp= topVar(i);
        }
        else if(e == True() && topVar(i)==topVar(t) && topVar(i)>topVar(t))
        {
            i_temp= topVar(e);
            t_temp= t;
            e_temp= topVar(i);
        }
        //ite(F,G,not(G))=ite(G,F,not(F))
        else if (isVariable(t) && isVariable(e) && topVar(i)> topVar(t))
        {
            i_temp=t;
            t_temp=i;
            e_temp=neg(i);
        }
        else if(isVariable(t) && isVariable(e) && topVar(i)==topVar(t) && i>t)
        {
            i_temp=t;
            t_temp=i;
            e_temp=neg(i);
        }
        */

    i=i_temp;
    t=t_temp;
    e=e_temp;
    size_t CompKey = CalcCompKey(i,t,e);
    if(COMPTable.find(CompKey)!=COMPTable.end())
    {
        return COMPTable[CompKey];
    }


    //}

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

    //the results of the prevision calculations is used to recursively (call the same function with itself)
    //call the ite-function.
    BDD_ID rHigh = ite(i_co_true,t_co_true,e_co_true);
    BDD_ID rLow = ite(i_co_false,t_co_false,e_co_false);

    //if rHigh and rLow equal, the ID representing them is returned.
    if (rHigh==rLow)
    {
        BDD_ID rReturn =rHigh;
        return rHigh;//return rHigh;
    }
    //Else we call the function find_or_add_unique_table, which checks the unique table for
    //Entry with TopVar for i,t,e , rLow (LowSuccesor) and rHigh (HighSuccesor).
    //If the unique table has entry, we return the id of entry, else we create a new entry,
    //and return the id of the new entry.
    BDD_ID r = find_or_add_unique_table(ID_of_TopVar_temp,rLow,rHigh);

    //we update the computed table with the i,t,e pattern, to safe that we already computed it.
    update_computed_table(CompKey,r);

    return r;
}
//Pseudo-Code was given in the documentation
BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    //Check if f is a constant (true or false)
    if (isConstant(f) || isConstant(x))
    {
        return f;
    }

    if (topVar(f)==x)
    {
        return GetHigh(f);
    }
    if (topVar(f)>x)
    {
        return f;
    }
        //Implementation of the given Pseudocode.
        size_t key = CalcCoFactorKey(f,x);
        if(CoFactorTrue_hash.find(key)==CoFactorTrue_hash.end())
        {
            BDD_ID T = coFactorTrue(GetHigh(f), x);
            BDD_ID F = coFactorTrue(GetLow(f), x);
            BDD_ID ID_ite= ite(topVar(f),T, F);
            CoFactorTrue_hash[key]=ID_ite;
            return ID_ite;
        }
        return CoFactorTrue_hash[key];
}
//Pseudo-Code was given in the documentation
BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    if (isConstant(f) || isConstant(x))
    {
        return f;
    }
    if (topVar(f)==x)
    {
        return GetLow(f);
    }
    if (topVar(f)>x)
    {
        return f;
    }
    size_t key = CalcCoFactorKey(f,x);
    if(CoFactorTrue_hash.find(key)==CoFactorTrue_hash.end())
    {
        BDD_ID T = coFactorFalse(GetHigh(f), x);
        BDD_ID F = coFactorFalse(GetLow(f), x);
        BDD_ID ID_ite= ite(topVar(f),T, F);
        CoFactorTrue_hash[key]=ID_ite;
        return ID_ite;
    }
    return CoFactorTrue_hash[key];
}
//The following 2 functions describe behaviour if x isn't described.
//basically if just call the function coFactorTrue/False with one parameter, we call the function
// with two parameter and set the second one to the top Variable of the first paramter.
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
//an4d given on slide 2-15 f and g = ite(f,g,0)
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
//same logic again. f nor g =ite(f,g,neg(g))
//if neg(f) is true , output g, else output neg(g)
BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{
    return ite(a,b,neg(b));
}

//Outputs every ID of TopVar that is reachable by Node with ID root
std::string Manager::getTopVarName(const BDD_ID &root)
{
    BDD_ID top_temp = BDDTable[root].TopVar_Entry;
    return BDDTable[top_temp].Varname_Entry;
}

//search for nodes that are connected to root.
void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    //root itself is always reachable
    nodes_of_root.insert(root);
    if (isConstant(root))
    {
        return;
    }
    else
    {
        //We call findNodes again with high and low of variable, we use them as new root.
        findNodes(BDDTable[root].High_Entry, nodes_of_root);
        findNodes(BDDTable[root].Low_Entry, nodes_of_root);
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{
    if (isConstant(root))
    {
        return;
    }
    else
    {
        //if root is no leaf node insert the TopVar of root as first vars of root.
        vars_of_root.insert(BDDTable[root].TopVar_Entry);
        //We call findVars again with high and low of variable, we use them as new root.
        findVars(BDDTable[root].High_Entry, vars_of_root);
        findVars(BDDTable[root].Low_Entry, vars_of_root);
    }
}

//returns the size of the BDDTable.
size_t Manager::uniqueTableSize()
{
    return BDDTable.size();
}

//add an entry to the BDDTable (unique table), which is a std::vector. The .push_back adds object of class BDDEntry
//to the vector. there the described the constructor within Manager.h
BDD_ID Manager::createNode(std::string NodeName, BDD_ID NodeID, BDD_ID NodeLow, BDD_ID NodeHigh, BDD_ID NodeTop)
{
    // add element to the end of BDDTable, we use class constructor for that
    //BDDTable.push_back({NodeName, NodeID, NodeHigh, NodeLow, NodeTop});
    BDDTable[NodeID]={NodeName, NodeID, NodeHigh, NodeLow, NodeTop};
    size_t BDDKey= CalcCompKey(NodeTop,NodeLow,NodeHigh);
    BDDTable_hash[BDDKey]=NodeID;
    //ID is always position -1
    return uniqueTableSize() -1;
}

//returns the HighSuccesors of Entry with ID ID
BDD_ID Manager::GetHigh(BDD_ID ID)
{
    return BDDTable[ID].High_Entry;
}

//returns the LowSuccesors of Entry with ID ID
BDD_ID Manager::GetLow(BDD_ID ID)
{
    return BDDTable[ID].Low_Entry;
}

//Check if the unique table has Entry with ID x and LowSuccesor rLow and HighSuccesor rHigh.
//if true, return the ID of the Entry, else call createNode and return the ID of the new Node.
BDD_ID Manager::find_or_add_unique_table(BDD_ID x,BDD_ID rLow,BDD_ID rHigh)
{
    //for loop to check every element of the unique table
    size_t BDDKey= CalcCompKey(x,rLow,rHigh);
    if(BDDTable_hash.find(BDDKey)==BDDTable_hash.end())
    {
        return createNode("",uniqueTableSize(),rLow,rHigh,x);
    }
    return BDDTable_hash[BDDKey];
    /*
    for (auto & loop_object : BDDTable)
    {
        if (loop_object.TopVar_Entry==x && loop_object.Low_Entry==rLow && loop_object.High_Entry==rHigh)
        {
            return loop_object.BDD_ID_Entry;
        }
    }
    return createNode("",uniqueTableSize(),rLow,rHigh,x);
     */
}

//Add Entry to Computed Table with the ID f,g,h and r.
void Manager::update_computed_table(size_t Key, BDD_ID r)
{
    COMPTable[Key]=r;
}

size_t Manager::CalcCompKey(BDD_ID f, BDD_ID g, BDD_ID h)
{
    size_t key = (f<<42)+(g<<21)+h;
    return key;
}

//Computation of hash-key for unordered map of Cofactor is less computation intense as the CalcCompkey
size_t Manager::CalcCoFactorKey(BDD_ID f, BDD_ID x)
{
    size_t key = (f<<32)+x;
    return key;
}

