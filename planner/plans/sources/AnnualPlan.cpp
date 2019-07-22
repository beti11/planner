#include "../headers/AnnualPlan.h"

AnnualPlan::AnnualPlan( std::string description, std::string category, int year )
        : Plan( std::move( description ), std::move( category ) ), year {year}
{
}

AnnualPlan::AnnualPlan( int id, std::string description, std::string category, int year, bool done )
        : Plan( id, std::move( description ), std::move( category ), done ), year {year}
{
}

int AnnualPlan::getYear() const
{
    return year;
}

