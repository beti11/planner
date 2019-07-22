#include <sstream>
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include "../headers/SQLService.h"
#include "../../plans/headers/Plan.h"
#include "../../plans/headers/MonthlyPlan.h"
#include "../../plans/headers/WeeklyPlan.h"
#include "../../plans/headers/DailyPlan.h"
#include "../../plans/headers/AnnualPlan.h"

using namespace std;
using namespace sql;

SQLService::SQLService()
{
    connect();
}

SQLService::~SQLService()
{
    connectionPtr->close();
}

void SQLService::connect()
{
    string myLogin = "root";
    string myPassword = "aa";
    string schema = "planner2";
    try
    {
        //initialize a connectionPtr
        driverPtr = get_driver_instance();
        connectionPtr = unique_ptr < sql::Connection >( driverPtr->connect( "localhost", myLogin, myPassword ) );
        //connect to the MySQL database
        connectionPtr->setSchema( schema );

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::readAllDailyPlans( vector < shared_ptr < Plan >> &plans, const boost::gregorian::date &date )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "SELECT * FROM daily WHERE date = ? ORDER BY done, category" ) );
        string d = boost::gregorian::to_iso_extended_string( date );
        preparedStatementPtr->setString( 1, d );
        preparedStatementPtr->execute();
        resultSetPtr = unique_ptr < ResultSet >( preparedStatementPtr->getResultSet() );

        int id;
        string description;
        string category;
        bool done;

        while ( resultSetPtr->next() )
        {
            id = resultSetPtr->getInt( "id" );
            description = resultSetPtr->getString( "description" );
            category = resultSetPtr->getString( "category" );
            done = resultSetPtr->getBoolean( "done" );
            plans.emplace_back( std::unique_ptr < Plan >( new DailyPlan( id, description, category, date, done ) ) );
        }

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::readAllWeeklyPlans( std::vector < shared_ptr < Plan >> &plans, int weekNumber )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "SELECT * FROM weekly WHERE weekNo = ? ORDER BY done, category" ) );
        preparedStatementPtr->setInt( 1, weekNumber );
        preparedStatementPtr->execute();
        resultSetPtr = unique_ptr < ResultSet >( preparedStatementPtr->getResultSet() );

        int id;
        string description;
        string category;
        bool done;

        while ( resultSetPtr->next() )
        {
            id = resultSetPtr->getInt( "id" );
            description = resultSetPtr->getString( "description" );
            category = resultSetPtr->getString( "category" );
            done = resultSetPtr->getBoolean( "done" );
            plans.emplace_back(
                    std::unique_ptr < Plan >( new WeeklyPlan( id, description, category, weekNumber, done ) ) );
        }

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::readAllMonthlyPlans( std::vector < shared_ptr < Plan >> &plans, const string &month, int year )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "SELECT * FROM monthly WHERE month = ? && year = ? ORDER BY done, category" ) );
        preparedStatementPtr->setString( 1, month );
        preparedStatementPtr->setInt( 2, year );
        preparedStatementPtr->execute();
        resultSetPtr = unique_ptr < ResultSet >( preparedStatementPtr->getResultSet() );

        int id;
        string description;
        string category;
        bool done;
        int year;

        while ( resultSetPtr->next() )
        {
            id = resultSetPtr->getInt( "id" );
            description = resultSetPtr->getString( "description" );
            category = resultSetPtr->getString( "category" );
            done = resultSetPtr->getBoolean( "done" );
            year = resultSetPtr->getInt( "year" );
            plans.emplace_back(
                    unique_ptr < Plan >( new MonthlyPlan( id, description, category, month, done, year ) ) );
        }

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::readAllAnnualPlans( std::vector < shared_ptr < Plan >> &plans, int year )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "SELECT * FROM annual WHERE year = ? ORDER BY done, category" ) );
        preparedStatementPtr->setInt( 1, year );
        preparedStatementPtr->execute();
        resultSetPtr = unique_ptr < ResultSet >( preparedStatementPtr->getResultSet() );

        int id;
        string description;
        string category;
        bool done;

        while ( resultSetPtr->next() )
        {
            id = resultSetPtr->getInt( "id" );
            description = resultSetPtr->getString( "description" );
            category = resultSetPtr->getString( "category" );
            done = resultSetPtr->getBoolean( "done" );
            plans.emplace_back( unique_ptr < Plan >( new AnnualPlan( id, description, category, year, done ) ) );
        }

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::deletePlan( const shared_ptr < Plan > &plan )
{

    string table = chooseTableName( plan );

    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >(
                connectionPtr->prepareStatement( "DELETE FROM " + table + " WHERE id = ? " ) );
        preparedStatementPtr->setInt( 1, plan->getId() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::addPlan( const shared_ptr < DailyPlan > &plan )
{

    string strDate = to_iso_extended_string( plan->getDate() );

    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "INSERT INTO daily (description, category, date ) VALUES (?, ?, ?)" ) );
        preparedStatementPtr->setString( 1, plan->getDescription() );
        preparedStatementPtr->setString( 2, plan->getCategory() );
        preparedStatementPtr->setString( 3, strDate );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::addPlan( const shared_ptr < WeeklyPlan > &plan )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "INSERT INTO weekly (description, category, weekNo ) VALUES (?, ?, ?)" ) );
        preparedStatementPtr->setString( 1, plan->getDescription() );
        preparedStatementPtr->setString( 2, plan->getCategory() );
        preparedStatementPtr->setInt( 3, plan->getWeekNumber() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::addPlan( const shared_ptr < MonthlyPlan > &plan )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "INSERT INTO monthly (description, category, month, year ) VALUES (?, ?, ?, ?)" ) );
        preparedStatementPtr->setString( 1, plan->getDescription() );
        preparedStatementPtr->setString( 2, plan->getCategory() );
        preparedStatementPtr->setString( 3, plan->getMonth() );
        preparedStatementPtr->setInt( 4, plan->getYear() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::addPlan( const shared_ptr < AnnualPlan > &plan )
{
    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "INSERT INTO annual (description, category, year ) VALUES (?, ?, ?)" ) );
        preparedStatementPtr->setString( 1, plan->getDescription() );
        preparedStatementPtr->setString( 2, plan->getCategory() );
        preparedStatementPtr->setInt( 3, plan->getYear() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::markDone( const shared_ptr < Plan > &plan )
{
    string table = chooseTableName( plan );

    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >(
                connectionPtr->prepareStatement( "UPDATE " + table + " SET done = ? WHERE id=?" ) );
        preparedStatementPtr->setBoolean( 1, plan->isDone() );
        preparedStatementPtr->setInt( 2, plan->getId() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

void SQLService::updatePlan( const shared_ptr < Plan > &plan )
{
    string table = chooseTableName( plan );

    try
    {
        preparedStatementPtr = unique_ptr < sql::PreparedStatement >( connectionPtr->prepareStatement(
                "UPDATE " + table + " SET description=?, category=? WHERE id=?" ) );
        preparedStatementPtr->setInt( 3, plan->getId() );
        preparedStatementPtr->setString( 1, plan->getDescription() );
        preparedStatementPtr->setString( 2, plan->getCategory() );
        preparedStatementPtr->execute();

    }
    catch ( sql::SQLException &e )
    {
        printErrorInfo( e );
    }
}

std::string SQLService::chooseTableName( const std::shared_ptr < Plan > &plan )
{
    string table;
    if ( dynamic_pointer_cast < DailyPlan >( plan ) != nullptr )
    {
        table = "daily";
    }
    else if ( dynamic_pointer_cast < WeeklyPlan >( plan ) != nullptr )
    {
        table = "weekly";
    }
    else if ( dynamic_pointer_cast < MonthlyPlan >( plan ) != nullptr )
    {
        table = "monthly";
    }
    else if ( dynamic_pointer_cast < AnnualPlan >( plan ) != nullptr )
    {
        table = "annual";
    }

    return table;
}

void SQLService::printErrorInfo( const SQLException &e )
{
    cerr
            << "There is a problem with the database"
            << "\nMySQL error code: "
            << e.getErrorCode()
            << "\t"
            << e.what()
            << endl;
}