#ifndef PLANNER_WEEKLYPLAN_H
#define PLANNER_WEEKLYPLAN_H

#include "Plan.h"

class WeeklyPlan :
        public Plan
{
public:
    WeeklyPlan( std::string description, std::string category, int weekNumber );
    WeeklyPlan( int id, std::string description, std::string category, int weekNumber, bool done );
    ~WeeklyPlan() override = default;
    int getWeekNumber() const;
private:
    int weekNumber;
};

#endif //PLANNER_WEEKLYPLAN_H
