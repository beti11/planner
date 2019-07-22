#include "../headers/DailyPlan.h"

DailyPlan::DailyPlan( std::string description, std::string category, boost::gregorian::date date )
        : Plan( std::move( description ), std::move( category ) ), date {date}
{
}

DailyPlan::DailyPlan( int id, std::string description, std::string category, boost::gregorian::date date, bool done )
        : Plan( id, std::move( description ), std::move( category ), done ), date {date}
{
}

const boost::gregorian::date &DailyPlan::getDate() const
{
    return date;
}
