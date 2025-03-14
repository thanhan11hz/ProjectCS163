#ifndef Logic_hpp
#define Logic_hpp

#include "Step.hpp"

class Logic {
    protected:
        StepManager stepmanager;
    public:
        virtual ~Logic() = default;
        std::vector<int> value;
        virtual void initData() = 0;
        virtual void insertData() = 0;
        virtual void deleteData() = 0;
        virtual void searchData() = 0;
};

#endif