#ifndef PLANNER_DAILYPLAN_H
#define PLANNER_DAILYPLAN_H

#include "Plan.h"

class DailyPlan :
        public Plan
{
public:
    DailyPlan( std::string description, std::string category, boost::gregorian::date date );
    DailyPlan( int id, std::string description, std::string category, boost::gregorian::date date, bool done );
    ~DailyPlan() override = default;
    const boost::gregorian::date &getDate() const;
private:
    boost::gregorian::date date;
};

#endif //PLANNER_DAILYPLAN_H
