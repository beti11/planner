#ifndef PLANNER_ANNUALPLAN_H
#define PLANNER_ANNUALPLAN_H

#include "Plan.h"

class AnnualPlan :
        public Plan
{
public:
    AnnualPlan( std::string description, std::string category, int year );
    AnnualPlan( int id, std::string description, std::string category, int year, bool done );
    ~AnnualPlan() override = default;
    int getYear() const;
private:
    int year;
};

#endif //PLANNER_ANNUALPLAN_H
