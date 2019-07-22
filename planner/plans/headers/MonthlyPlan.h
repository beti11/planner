#ifndef PLANNER_MONTHLYPLAN_H
#define PLANNER_MONTHLYPLAN_H

#include "Plan.h"

class MonthlyPlan :
        public Plan
{
public:
    MonthlyPlan( std::string description, std::string category, std::string month, int year );
    MonthlyPlan( int id, std::string description, std::string category, std::string month, bool done, int year );
    ~MonthlyPlan() override = default;
    const std::string &getMonth() const;
    int getYear() const;
private:
    std::string month;
    int year;
};

#endif //PLANNER_MONTHLYPLAN_H
