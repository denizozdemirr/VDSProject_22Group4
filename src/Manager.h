// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"
#include <vector>
namespace ClassProject {

    const BDD_ID TrueID=1;
    const BDD_ID FalseID=0;
    //At the moment we assume a constant number of Variables, because we need to specify the size of the
    //class array somehow. We basically have 2 power number of vars that can represented in CNF abc... ~ab~c and so on
    //Additionally we always have entry True and False and function, that's why we add 3
    class BDDEntry
    {
    public:
        std::string Varname_Entry;
        BDD_ID BDD_ID_Entry;
        BDD_ID High_Entry;
        BDD_ID Low_Entry;
        BDD_ID TopVar_Entry;

        BDDEntry(std::string Label, BDD_ID ID, BDD_ID High, BDD_ID Low, BDD_ID Top)
        {
            Varname_Entry = Label;
            BDD_ID_Entry = ID;
            High_Entry = High;
            Low_Entry = Low;
            TopVar_Entry = Top;
        }
    };
    class ComputedEntry
    {
    public:
        BDD_ID f;
        BDD_ID g;
        BDD_ID h;
        BDD_ID r;

        ComputedEntry(BDD_ID F, BDD_ID G, BDD_ID H, BDD_ID R)
        {
            f = F;
            g = G;
            h = H;
            r = R;
        }
    };
    class Manager : public ManagerInterface
    {
    public:
        std::vector<BDDEntry> BDDTable;
        std::vector<ComputedEntry> COMPTable;

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
        void update_computed_table(BDD_ID f,BDD_ID g,BDD_ID h,BDD_ID r);


    };




}

#endif
