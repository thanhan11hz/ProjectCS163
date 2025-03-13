#ifndef Graph_hpp
#define Graph_hpp

#include "Logic.hpp"
#include "View.hpp"
class Graph : public Logic, public View {
    public:
        void init();
        void draw();
        void run();
};

#endif