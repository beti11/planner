#include "../headers/WeeklyPlan.h"

WeeklyPlan::WeeklyPlan( std::string description, std::string category, int weekNumber )
        : Plan( std::move( description ), std::move( category ) ), weekNumber {weekNumber}
{
}

WeeklyPlan::WeeklyPlan( int id, std::string description, std::string category, int weekNumber, bool done )
        : Plan( id, std::move( description ), std::move( category ), done ), weekNumber {weekNumber}
{
}

int WeeklyPlan::getWeekNumber() const
{
    return weekNumber;
}
