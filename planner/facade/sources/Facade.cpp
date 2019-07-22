#include <ncurses.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "../headers/Facade.h"
#include "../../sqlService/headers/SQLService.h"
#include "../../plans/headers/DailyPlan.h"
#include "../../plans/headers/WeeklyPlan.h"
#include "../../plans/headers/MonthlyPlan.h"
#include "../../plans/headers/AnnualPlan.h"
#include "../../plans/headers/Plan.h"
#include "../../planServices/headers/PlanService.h"
#include "../../views/headers/View.h"

using namespace std;
using namespace boost::gregorian;

//choose a plan which you want to change from a list
shared_ptr < Plan > Facade::choosePlanToChange()
{
    int planNumber = getView()->choosePlanToChange( plans.size() );
    return plans[planNumber];
}

//call the appropriate function depending on the option selected by the user in the menu
void Facade::performAppropriateAction( char ch )
{
    string description;
    string category;
    int week;

    switch ( ch )
    {
    case 'd':
        date = planService->getDateFromUser( view, true );
        printDailyPlans();
        break;
    case 'w':
        date = planService->getDateFromUser( view, true );
        printWeeklyPlans();
        break;
    case 'm':
        month = planService->getMonthFromUserForReadingPlans( view );
        year = planService->getYearFromUserForReadingPlans( view );
        printMonthlyPlans();
        break;
    case 'y':
        year = planService->getYearFromUserForReadingPlans( view );
        printAnnualPlans();
        break;
    case 'a':
        date = planService->getDateFromUser( view, false );
        planService->createDailyPlan( date, view );
        printDailyPlans();
        break;
    case 'e':
        date = planService->getDateFromUser( view, false );
        week = date.week_number();
        planService->createWeeklyPlan( week, view );
        printWeeklyPlans();
        break;
    case 'o':
        month = planService->getMonthFromUserForReadingPlans( view );
        year = planService->getYearFromUserForReadingPlans( view );
        planService->createMonthlyPlan( month, year, view );
        printMonthlyPlans();
        break;
    case 'n':
        year = planService->getYearFromUserForCreatingPlan( view );
        planService->createAnnualPlan( year, view );
        printAnnualPlans();
        break;
    case 'u':
        plan = choosePlanToChange();
        planService->updatePlan( plan, view );
        checkPlanType();
        break;
    case 'r':
        plan = choosePlanToChange();
        planService->removePlan( plan );
        checkPlanType();
        break;
    case 'k':
        plan = choosePlanToChange();
        planService->markDone( plan );
        checkPlanType();
        break;
    default:
        break;
    }
}

//print menu and plans for a current day
void Facade::start()
{
    boost::gregorian::date today = day_clock::local_day();
    setDate( today );

    printDailyPlans();
    getView()->createMenuWindow();
    getView()->createOutputWindow();
    getView()->createInputWindow();
}

//clear vector that contains plans and read to it plans for a new day
void Facade::printDailyPlans()
{
    plans.clear();

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->readAllDailyPlans( plans, date );

    view->createDailyPlansWindow( plans, date );

}

//clear vector that contains plans and read to it plans for a new week
void Facade::printWeeklyPlans()
{
    plans.clear();

    unique_ptr < SQLService > sqlService( new SQLService() );
    int weekNumber = date.week_number();
    sqlService->readAllWeeklyPlans( plans, weekNumber );

    view->createWeeklyPlansWindow( plans, date );

}

//clear vector that contains plans and read to it plans for a new month
void Facade::printMonthlyPlans()
{
    plans.clear();

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->readAllMonthlyPlans( plans, month, year );

    view->createMonthlyPlansWindow( plans, month, year );
}

//clear vector that contains plans and read to it plans for a new year
void Facade::printAnnualPlans()
{
    plans.clear();

    unique_ptr < SQLService > sqlService( new SQLService() );
    sqlService->readAllAnnualPlans( plans, year );

    view->createAnnualPlansWindow( plans, year );
}

void Facade::checkPlanType()
{
    if ( dynamic_pointer_cast < DailyPlan >( plan ) != nullptr )
    {
        printDailyPlans();
    }
    else if ( dynamic_pointer_cast < WeeklyPlan >( plan ) != nullptr )
    {
        printWeeklyPlans();
    }
    else if ( dynamic_pointer_cast < MonthlyPlan >( plan ) != nullptr )
    {
        printMonthlyPlans();
    }
    else if ( dynamic_pointer_cast < AnnualPlan >( plan ) != nullptr )
    {
        printAnnualPlans();
    }
}

Facade::Facade( unique_ptr < PlanService > planService, unique_ptr < View > view )
        : planService( std::move( planService ) ), view( std::move( view ) )
{}

const unique_ptr < View > &Facade::getView() const
{
    return view;
}

void Facade::setDate( const boost::gregorian::date &date )
{
    Facade::date = date;
}

int main()
{
    unique_ptr < View > view( new View() );
    unique_ptr < PlanService > planService( new PlanService() );
    unique_ptr < Facade > facade( new Facade( move( planService ), move( view ) ) );
    facade->start();

    while ( true )
    {
        char ch = facade->getView()->getMenuOptionFromUser();
        if ( ch == 'q' )
        {
            facade->getView()->exit();
            return 0;
        }
        else
        {
            facade->performAppropriateAction( ch );
        }

    }
}

