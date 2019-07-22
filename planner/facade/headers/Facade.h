#ifndef PLANNER_FACADE_H
#define PLANNER_FACADE_H

#include <boost/date_time/gregorian/gregorian.hpp>

class Plan;
class PlanService;
class View;

class Facade
{
public:
    Facade( std::unique_ptr < PlanService > planService, std::unique_ptr < View > view );
    void performAppropriateAction( char ch );
    void printDailyPlans();
    void printWeeklyPlans();
    void printMonthlyPlans();
    void printAnnualPlans();
    std::shared_ptr < Plan > choosePlanToChange();
    void setDate( const boost::gregorian::date &date );
    const std::unique_ptr < View > &getView() const;
    void start();
private:
    void checkPlanType();
    std::vector < std::shared_ptr < Plan>> plans;
    boost::gregorian::date date;
    std::string month;
    int year;
    std::shared_ptr < Plan > plan;
    std::unique_ptr < PlanService > planService;
    std::unique_ptr < View > view;
};

#endif //PLANNER_FACADE_H
