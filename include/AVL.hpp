#ifndef AVL_hpp
#define AVL_hpp

#include "Logic.hpp"
#include "View.hpp"

class AVL : public Logic, public View {
    public:
        void init();
        void draw(); 
        void run();
        void exit();
        void initData();
        void insertData();
        void deleteData();
        void searchData();
};

#endif