#ifndef PLANNER_PLANSERVICE_H
#define PLANNER_PLANSERVICE_H

#include <memory>
#include <vector>

class Plan;
class View;

class PlanService
{
public:
    static void createDailyPlan( const boost::gregorian::date &d, const std::unique_ptr < View > &view );
    static void createWeeklyPlan( int weekNumber, const std::unique_ptr < View > &view );
    static void createMonthlyPlan( const std::string &month, int year, const std::unique_ptr < View > &view );
    static void createAnnualPlan( int year, const std::unique_ptr < View > &view );
    static void updatePlan( const std::shared_ptr < Plan > &plan, const std::unique_ptr < View > &view );
    static void markDone( const std::shared_ptr < Plan > &plan );
    static void removePlan( const std::shared_ptr < Plan > &plan );
    static boost::gregorian::date getDateFromUser( const std::unique_ptr < View > &view, bool forReadingPlans );
    static std::string getMonthFromUserForReadingPlans( const std::unique_ptr < View > &view );
    static int getYearFromUserForCreatingPlan( const std::unique_ptr < View > &view );
    static int getYearFromUserForReadingPlans( const std::unique_ptr < View > &view );
private:
    static boost::gregorian::date getDateFromUserForReadingPlans( const std::unique_ptr < View > &view );
    static boost::gregorian::date getDateFromUserForCreatingPlan( const std::unique_ptr < View > &view );
    static bool isInt( const std::string &day, const std::string &month, const std::string &year );
};

#endif //PLANNER_PLANSERVICE_H
