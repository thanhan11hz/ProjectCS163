#ifndef Program_hpp
#define Program_hpp

#include <iostream>
#include "raylib.h"
#include "Menu.hpp"
#include "SLList.hpp"
#include "HTable.hpp"
#include "AVL.hpp"
#include "Graph.hpp"
#include "GlobalVar.hpp"
class Program {
    private:
        Menu menu;
        SLList list;
        HTable table;
        AVL tree;
        Graph graph;
    public:
        Program();
        void run();
    private:
        void eventProcessing();
        void drawing();
};

#endif