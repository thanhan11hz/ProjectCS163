#ifndef SLList_hpp
#define SLList_hpp

#include "Logic.hpp"
#include "View.hpp"
class SLList: public Logic, public View {
    public:
        void init();
        void draw();
        void run();
};

#endif