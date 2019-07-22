#ifndef PLANNER_SQLSERVICE_H
#define PLANNER_SQLSERVICE_H

#include <string>
#include <memory>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <vector>

class Plan;
class MonthlyPlan;
class AnnualPlan;
class WeeklyPlan;
class DailyPlan;

class SQLService
{
public:
    SQLService();
    ~SQLService();
    void readAllDailyPlans( std::vector < std::shared_ptr < Plan >> &plans, const boost::gregorian::date &d );
    void readAllWeeklyPlans( std::vector < std::shared_ptr < Plan >> &plans, int weekNumber );
    void readAllMonthlyPlans( std::vector < std::shared_ptr < Plan >> &plans, const std::string &month, int year );
    void readAllAnnualPlans( std::vector < std::shared_ptr < Plan >> &plans, int year );
    void deletePlan( const std::shared_ptr < Plan > &plan );
    void addPlan( const std::shared_ptr < DailyPlan > &plan );
    void addPlan( const std::shared_ptr < WeeklyPlan > &plan );
    void addPlan( const std::shared_ptr < MonthlyPlan > &plan );
    void addPlan( const std::shared_ptr < AnnualPlan > &plan );
    void updatePlan( const std::shared_ptr < Plan > &plan );
    void markDone( const std::shared_ptr < Plan > &plan );

private:
    sql::Driver *driverPtr;
    std::unique_ptr < sql::Connection > connectionPtr;
    std::unique_ptr < sql::ResultSet > resultSetPtr;
    std::unique_ptr < sql::PreparedStatement > preparedStatementPtr;
    void connect();
    static std::string chooseTableName( const std::shared_ptr < Plan > &plan );
    static void printErrorInfo( const sql::SQLException &e );
};

#endif //PLANNER_SQLSERVICE_H
