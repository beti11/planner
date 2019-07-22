#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "../../plans/headers/Plan.h"
#include "../headers/PlanService.h"
#include "../../sqlService/headers/SQLService.h"
#include "../../plans/headers/DailyPlan.h"
#include "../../plans/headers/WeeklyPlan.h"
#include "../../plans/headers/MonthlyPlan.h"
#include "../../plans/headers/AnnualPlan.h"
#include "../../plans/headers/Plan.h"
#include "../../views/headers/View.h"

using namespace std;
using namespace boost::gregorian;

void PlanService::createDailyPlan( const date &d, const unique_ptr < View > &view )
{
    string description = view->getDescriptionFromUser();
    string category = view->getCategoryFromUser();
    unique_ptr < DailyPlan > plan( new DailyPlan( description, category, d ) );

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->addPlan( move( plan ) );
}

void PlanService::createWeeklyPlan( int weekNumber, const unique_ptr < View > &view )
{
    string description = view->getDescriptionFromUser();
    string category = view->getCategoryFromUser();
    unique_ptr < WeeklyPlan > plan( new WeeklyPlan( description, category, weekNumber ) );

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->addPlan( move( plan ) );
}

void PlanService::createMonthlyPlan( const string &month, int year, const unique_ptr < View > &view )
{
    string description = view->getDescriptionFromUser();
    string category = view->getCategoryFromUser();
    unique_ptr < MonthlyPlan > plan( new MonthlyPlan( description, category, month, year ) );

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->addPlan( move( plan ) );
}

void PlanService::createAnnualPlan( int year, const unique_ptr < View > &view )
{
    string description = view->getDescriptionFromUser();
    string category = view->getCategoryFromUser();
    unique_ptr < AnnualPlan > plan( new AnnualPlan( description, category, year ) );

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->addPlan( move( plan ) );
}

//change description or category of a plan
void PlanService::updatePlan( const shared_ptr < Plan > &plan, const unique_ptr < View > &view )
{
    string description = view->getDescriptionFromUser();
    string category = view->getCategoryFromUser();
    plan->setDescription( description );
    plan->setCategory( category );

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->updatePlan( plan );
}

//mark a plan as done or uncheck if you make a mistake
void PlanService::markDone( const shared_ptr < Plan > &plan )
{
    plan->setDone( !plan->isDone() );
    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->markDone( plan );
}

void PlanService::removePlan( const shared_ptr < Plan > &plan )
{
    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->deletePlan( plan );
}

string PlanService::getMonthFromUserForReadingPlans( const unique_ptr < View > &view )
{
    array < string, 12 > monthsArray {"January", "February", "March", "April", "May", "June", "July", "August",
                                      "September",
                                      "November", "October", "December"};
    string month;
    do
    {
        month = view->getMonthFromUser();

        //if user didn't enter a month - set it to the current one
        if ( month.empty() )
        {
            month = day_clock::local_day().month().as_long_string();
            return month;
        }

        //check whether a month entered by user is valid
    } while ( find( std::begin( monthsArray ), std::end( monthsArray ), month ) == std::end( monthsArray ) );

    return month;
}

int PlanService::getYearFromUserForReadingPlans( const unique_ptr < View > &view )
{
    int year;
    bool isInt;
    string sYear;
    do
    {
        sYear = view->getYearFromUser();
        //if user didn't enter a year - set it to the current one
        if ( sYear.empty() )
        {
            year = day_clock::local_day().year();
            return year;
        }
            //check whether a year is an integer type
        else
        {
            isInt = ( sYear.find_first_not_of( "0123456789" ) == std::string::npos );
        }

    } while ( !isInt );
    year = stoi( sYear );

    return year;
}

int PlanService::getYearFromUserForCreatingPlan( const unique_ptr < View > &view )
{
    int year;
    bool isInt;
    string sYear;
    do
    {
        sYear = view->getYearFromUser();
        //if user didn't enter a year - set it to the next one
        if ( sYear.empty() )
        {
            years addedYear( 1 );
            year = ( day_clock::local_day() + addedYear ).year();
            return year;
        }
        //check whether a year is an integer type
        isInt = ( sYear.find_first_not_of( "0123456789" ) == std::string::npos );

    } while ( !isInt || ( year = stoi( sYear ) ) < day_clock::local_day().year() );

    return year;
}

//check whether a user entered integer values for a day, month and year
bool PlanService::isInt( const std::string &day, const std::string &month, const std::string &year )
{
    bool isDayInt;
    bool isMonthInt;
    bool isYearInt;

    isDayInt = ( day.find_first_not_of( "0123456789" ) == std::string::npos );
    isMonthInt = ( month.find_first_not_of( "0123456789" ) == std::string::npos );
    isYearInt = ( year.find_first_not_of( "0123456789" ) == std::string::npos );

    return isDayInt && isMonthInt && isYearInt;
}

date PlanService::getDateFromUser( const unique_ptr < View > &view, bool forReadingPlans )
{
    date date;
    bool exceptionFlag = true;

    //get a day, month and year from a user; repeat until a date is correct
    while ( exceptionFlag )
    {
        try
        {
            if ( forReadingPlans )
            {
                date = getDateFromUserForReadingPlans( view );
            }
            else
            {
                date = getDateFromUserForCreatingPlan( view );
            }
            exceptionFlag = false;
        }
        catch ( out_of_range & )
        {
        }
    }
    return date;
}

//to read plans a user can choose past, current or future date
boost::gregorian::date PlanService::getDateFromUserForReadingPlans( const unique_ptr < View > &view )
{
    int day, month, year;
    date d;
    array < string, 3 > arrayDate; //contains day,month,year

    do
    {
        arrayDate = view->getDateFromUser();
        if ( arrayDate[0].length() > 0 && arrayDate[1].length() > 0 && arrayDate[2].length() > 0 &&
             isInt( arrayDate[0], arrayDate[1], arrayDate[2] ) )
        {
            day = stoi( arrayDate[0] );
            month = stoi( arrayDate[1] );
            year = stoi( arrayDate[2] );

            //when a date is invalid (e.g. 30.02) throw an exception
            try
            {
                d = date( year, month, day );

            }
            catch ( out_of_range & )
            {
                throw;
            }
        }
            //if user didn't enter a day AND a month AND a year - set a date to the current one
        else if ( arrayDate[0].length() == 0 && arrayDate[1].length() == 0 && arrayDate[2].length() == 0 )
        {
            d = day_clock::local_day();
        }
        //if user didn't enter a day OR a month OR a year OR didn't enter integer values  - repeat
    } while ( d.is_not_a_date() );

    return d;
}

//to create a new plan a user can choose only current or future date
boost::gregorian::date PlanService::getDateFromUserForCreatingPlan( const unique_ptr < View > &view )
{
    int day, month, year;
    date d;
    array < string, 3 > arrayDate; //contains day,month,year

    do
    {
        arrayDate = view->getDateFromUser();
        if ( arrayDate[0].length() > 0 && arrayDate[1].length() > 0 && arrayDate[2].length() > 0 &&
             isInt( arrayDate[0], arrayDate[1], arrayDate[2] ) )
        {
            day = stoi( arrayDate[0] );
            month = stoi( arrayDate[1] );
            year = stoi( arrayDate[2] );

            //when a date is invalid (e.g. 30.02) throw an exception
            try
            {
                d = date( year, month, day );

            }
            catch ( out_of_range & )
            {
                throw;
            }
        }
            //if user didn't enter a day AND a month AND a year - set a date to the next one
        else if ( arrayDate[0].length() == 0 && arrayDate[1].length() == 0 && arrayDate[2].length() == 0 )
        {
            days addedDays( 1 );
            d = day_clock::local_day() + addedDays;
        }
        //if user didn't enter a day OR a month OR a year OR didn't enter integer values OR entered a date from the past - repeat
    } while ( d.is_not_a_date() || d < day_clock::local_day() );

    return d;
}



