#ifndef Graph_hpp
#define Graph_hpp

#include "Logic.hpp"
#include "View.hpp"
class Graph : public Logic, public View {
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