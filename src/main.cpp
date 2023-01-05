//
// Created by Carolina P. Nogueira 2016
//

#include <iostream>
#include <string>
#include "Manager.h"
//we need to declare which namespace we are using
//Manager defines the functions we are using (see Manager.h)
using ClassProject::Manager;
//We are using the datatype BDD_ID withing in the main.cpp, so we need to add it's definitions too.
using ClassProject::BDD_ID;
int main(int argc, char* argv[])
{
    //f=(a+b)*c*d
    //first of all we need to create an manager Object.
    Manager m;

    //afterwards  we need to create the variables a to d
    BDD_ID a=m.createVar("a");
    BDD_ID b=m.createVar("b");
    BDD_ID c=m.createVar("c");
    BDD_ID d=m.createVar("d");
    //implementation of f. f=and2(and2(c,d),or2(a,b))=and2(or2(a,b),and2(c,d))
    BDD_ID f=m.and2(m.and2(c,d),m.or2(a,b));

    //output of unique table to check if table equals to the given one in the example
    //first get the size of the unique_table to know how many rows we need to output
    int size_unique_table=int(m.uniqueTableSize());
    //take care. the first row is always 0 NOT! 1
    for(int row=0;row<size_unique_table;row++)
    {
        // first output the row we are in right now (starts with0)
        std::cout << row;
        //this line does the tab
        std::cout << "  ";
        //output the High Variable of the row. We need to cast the datatype!
        //row is datatype int (integer) but the function GetHigh expects an input of datatype BDD_ID.
        //So we change the Datatype by adding the dataype().
        std::cout << m.GetHigh(BDD_ID (row));
        std::cout << "  ";
        //Output Low Variable of row
        std::cout << m.GetLow(BDD_ID (row));
        std::cout << "  ";
        //Output Lop Variable of row
        std::cout << m.topVar(BDD_ID (row));
        std::cout << "  "<< std::endl;
    }
    std::cout << "Implementation done" << std::endl;
}

