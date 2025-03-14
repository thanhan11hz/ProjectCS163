#ifndef Logic_hpp
#define Logic_hpp

#include "Step.hpp"

class Logic {
    protected:
        StepManager stepmanager;
    public:
        virtual ~Logic() = default;
        void initData();
        void insertData();
        void removeData();
        void searchData();
};

#endif