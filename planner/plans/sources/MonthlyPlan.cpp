#include "../headers/MonthlyPlan.h"

MonthlyPlan::MonthlyPlan( std::string description, std::string category, std::string month, int year )
        : Plan( std::move( description ), std::move( category ) ), month {std::move( month )}, year {year}
{
}

MonthlyPlan::MonthlyPlan( int id, std::string description, std::string category, std::string month, bool done,
                          int year )
        : Plan( id, std::move( description ), std::move( category ), done ), month {std::move( month )}, year {year}
{
}

const std::string &MonthlyPlan::getMonth() const
{
    return month;
}

int MonthlyPlan::getYear() const
{
    return year;
}

