#ifndef HTable_hpp
#define HTable_hpp

#include "Logic.hpp"
#include "View.hpp"
class HTable: public Logic, public View {
    public:
        void init();
        void draw();
        void run();
};

#endif