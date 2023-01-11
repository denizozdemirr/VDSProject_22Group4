// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"
#include <vector>
#include <unordered_map>
#include <boost/unordered_map.hpp>
namespace ClassProject {

    const BDD_ID TrueID=1;
    const BDD_ID FalseID=0;
    //At the moment we assume a constant number of Variables, because we need to specify the size of the
    //class array somehow. We basically have 2 power number of vars that can represented in CNF abc... ~ab~c and so on
    //Additionally we always have entry True and False and function, that's why we add 3
    struct BDDEntry
    {
        std::string Varname_Entry;
        BDD_ID BDD_ID_Entry;
        BDD_ID High_Entry;
        BDD_ID Low_Entry;
        BDD_ID TopVar_Entry;
    };
    class Manager : public ManagerInterface
    {
    private:
        //std::vector<BDDEntry> BDDTable;
        boost::unordered_map< size_t, BDDEntry> BDDTable;
        boost::unordered_map< size_t, BDD_ID> COMPTable;
        boost::unordered_map< size_t, BDD_ID> BDDTable_hash;
        boost::unordered_map< size_t, BDD_ID> CoFactorTrue_hash;
        boost::unordered_map< size_t, BDD_ID> CoFactorFalse_hash;
    public:
        Manager();
        ~Manager();

        BDD_ID createVar(const std::string &label) override;

        const BDD_ID &True() override;

        const BDD_ID &False() override;

        bool isConstant(BDD_ID f) override;

        bool isVariable(BDD_ID x) override;

        BDD_ID topVar(BDD_ID f) override;

        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorTrue(BDD_ID f) override;

        BDD_ID coFactorFalse(BDD_ID f) override;

        BDD_ID neg(BDD_ID a) override;

        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        std::string getTopVarName(const BDD_ID &root) override;

        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

        size_t uniqueTableSize() override;
        BDD_ID createNode(std::string NodeName, BDD_ID NodeID, BDD_ID NoteLow, BDD_ID NoteHigh, BDD_ID NoteTop);
        BDD_ID GetHigh(BDD_ID ID);
        BDD_ID GetLow(BDD_ID ID);
        BDD_ID find_or_add_unique_table(BDD_ID x,BDD_ID rLow,BDD_ID rHigh);
        void update_computed_table(size_t Key,BDD_ID r);
        size_t CalcCompKey(BDD_ID f,BDD_ID g,BDD_ID h);
        size_t CalcCoFactorKey(BDD_ID f, BDD_ID x);

        //int HashGetCompKey(int F, int G, int H);


    };




}

#endif
