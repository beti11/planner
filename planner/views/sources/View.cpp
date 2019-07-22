#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include "../headers/View.h"
#include "../../plans/headers/Plan.h"

using namespace std;
using namespace boost::gregorian;

View::View()
{
    //if default terminal window size is less than needed, resize it; COLS, LINES - variables managed by ncurses library
    if ( COLS < lineWidth || LINES < lineWidth )
    {
        system( ( "resize -s " + to_string( rows ) + " " + to_string( lineWidth ) ).c_str() );
    }
    initscr();
    noecho();
    cbreak();
    curs_set( 0 );
    start_color();
}

void View::exit()
{
    delwin( plansWindow );
    delwin( menuWindow );
    delwin( outputWindow );
    delwin( inputWindow );
    endwin();
}

//create window that contains a list of plans
void View::createPlansWindow( const vector < shared_ptr < Plan>> &plans )
{
    int maxNumberOfPlans = 60;
    int firstPlansWindowLinePosition = 8;
    int firstColumn = 0;

    //clean window contains old data before creating a new one
    string clearLine( lineWidth, ' ' );
    for ( int j = 0; j < maxNumberOfPlans; ++j )
    {
        mvprintw( firstPlansWindowLinePosition + j, firstColumn, clearLine.c_str() );
    }

    //create a window
    int rows = 4 + plans.size();
    int columns = lineWidth;
    plansWindow = newwin( rows, columns, firstPlansWindowLinePosition, firstColumn );
    //make window's borders invisible
    int space = ( int )' ';
    wborder( plansWindow, space, space, space, space, space, space, space, space );

    //print a line with columns' names
    init_pair( 2, COLOR_WHITE, COLOR_BLUE );
    wattron( plansWindow, A_BOLD );
    wattron( plansWindow, COLOR_PAIR( 2 ) );
    int y0 = 2;
    int x0 = 0;
    mvwprintw( plansWindow, y0, x0, createHeaderLine().c_str() );
    wattroff( plansWindow, A_BOLD );
    wattroff( plansWindow, COLOR_PAIR( 2 ) );

    //print plans
    for ( unsigned int i = 0; i < plans.size(); ++i )
    {
        mvwprintw( plansWindow, i + 3, 0, plans[i]->print().c_str() );
    }

    refresh();
}

//add to the plans window part that is specific for daily plans
void View::createDailyPlansWindow( const vector < shared_ptr < Plan>> &plans, boost::gregorian::date date )
{
    //print plans
    createPlansWindow( plans );

    //print first line that contains information about day of a week and date
    wattron( plansWindow, A_BOLD );
    wattron( plansWindow, COLOR_PAIR( 2 ) );
    mvwprintw( plansWindow, 1, 0, date.day_of_week().as_short_string() );
    mvwprintw( plansWindow, 1, 3, "   " );
    mvwprintw( plansWindow, 1, 5, to_iso_extended_string( date ).c_str() );
    wattroff( plansWindow, A_BOLD );
    wattroff( plansWindow, COLOR_PAIR( 2 ) );

    wrefresh( plansWindow );
}

//add to the plans window part that is specific for weekly plans
void View::createWeeklyPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, boost::gregorian::date date )
{

    createPlansWindow( plans );

    //print first line that contains information about a week
    wattron( plansWindow, A_BOLD );
    wattron( plansWindow, COLOR_PAIR( 2 ) );
    mvwprintw( plansWindow, 1, 0, to_iso_extended_string( previous_weekday( date, greg_weekday( Monday ) ) ).c_str() );
    mvwprintw( plansWindow, 1, 10, " - " );
    mvwprintw( plansWindow, 1, 13, to_iso_extended_string( next_weekday( date, greg_weekday( Sunday ) ) ).c_str() );
    wattroff( plansWindow, A_BOLD );
    wattroff( plansWindow, COLOR_PAIR( 2 ) );

    wrefresh( plansWindow );
}

//add to the plans window part that is specific for monthly plans
void
View::createMonthlyPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, const string &month, int year )
{
    createPlansWindow( plans );

    //print first line that contains information about a month
    wattron( plansWindow, A_BOLD );
    wattron( plansWindow, COLOR_PAIR( 2 ) );
    mvwprintw( plansWindow, 1, 0, month.c_str() );
    mvwprintw( plansWindow, 1, 3, "   " );
    mvwprintw( plansWindow, 1, 5, to_string( year ).c_str() );
    wattroff( plansWindow, A_BOLD );
    wattroff( plansWindow, COLOR_PAIR( 2 ) );

    wrefresh( plansWindow );
}

//add to the plans window part that is specific for annual plans
void View::createAnnualPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, int year )
{
    createPlansWindow( plans );

    //print first line that contains information about a year
    wattron( plansWindow, A_BOLD );
    wattron( plansWindow, COLOR_PAIR( 2 ) );
    mvwprintw( plansWindow, 1, 0, to_string( year ).c_str() );
    wattroff( plansWindow, A_BOLD );
    wattroff( plansWindow, COLOR_PAIR( 2 ) );

    wrefresh( plansWindow );
}

//create a window that contains menu
void View::createMenuWindow()
{
    //create a window
    int rows = 4;
    int columns = lineWidth;
    int y0 = 0;
    int x0 = 0;
    menuWindow = newwin( rows, columns, y0, x0 );
    //make window's borders invisible
    int space = ( int )' ';
    wborder( menuWindow, space, space, space, space, space, space, space, space );

    //print menu
    wattron( menuWindow, A_BOLD );
    wattron( menuWindow, COLOR_PAIR( 2 ) );
    mvwprintw( menuWindow, y0, x0, "choose the option:" );
    wattroff( menuWindow, A_BOLD );
    wattroff( menuWindow, COLOR_PAIR( 2 ) );
    mvwprintw( menuWindow, y0 + 1, x0,
               "print daily plans\t\t\tprint weekly plans\t\t\tprint monthly plans\t\t\tprint annual plans" );
    mvwprintw( menuWindow, y0 + 2, x0,
               "add a daily plan\t\t\tadd a weekly plan\t\t\tadd a monthly plan\t\t\tadd an annual plan" );
    mvwprintw( menuWindow, y0 + 3, x0,
               "update current entry\t\t\tremove an entry\t\t\t\tmark a plan done\t\t\tquit the program" );

    refresh();
}

//create a window that contains prompts for user
void View::createOutputWindow()
{
    int rows = 2;
    int columns = lineWidth;
    int y0 = 5;
    int x0 = 0;
    outputWindow = newwin( rows, columns, y0, x0 );
    //make window's borders invisible
    int space = ( int )' ';
    wborder( outputWindow, space, space, space, space, space, space, space, space );
    refresh();
}

//create window that contains data entered by a user
void View::createInputWindow()
{
    int rows = 1;
    int columns = lineWidth;
    int y0 = 7;
    int x0 = 0;
    inputWindow = newwin( rows, columns, y0, x0 );
    //make window's borders invisible
    int space = ( int )' ';
    wborder( inputWindow, space, space, space, space, space, space, space, space );
    refresh();
}

//clear text in a box for user's input
void View::clearInputWindow()
{
    string clearLine( lineWidth, ' ' );
    int y0 = 0;
    int x0 = 0;
    mvwprintw( inputWindow, y0, x0, clearLine.c_str() );
    wrefresh( inputWindow );
}

//clear text in a box that contains prompts for user
void View::clearOutputWindow()
{
    string clearLine( lineWidth, ' ' );
    int y0 = 0;
    int x0 = 0;
    mvwprintw( outputWindow, y0, x0, clearLine.c_str() );
    mvwprintw( outputWindow, y0 + 1, x0, clearLine.c_str() );
    wrefresh( outputWindow );
}

//create line that contains columns' names
string View::createHeaderLine()
{
    int idLength = 5;
    int descLength = 102;
    int catLength = 28;
    int doneLength = 5;

    stringstream buffer;
    buffer
            << setw( idLength )
            << left
            << "id"
            << setw( descLength )
            << left
            << "description"
            << setw( catLength )
            << left
            << "category"
            << setw( doneLength )
            << left
            << boolalpha
            << "done  ";

    return buffer.str();
}

//get plan description from a user
std::string View::getDescriptionFromUser()
{
    char cstrDescription[101];
    string description;

    echo();
    do
    {
        mvwprintw( outputWindow, 0, 0, "Enter a description." );
        wrefresh( outputWindow );
        mvwgetnstr( inputWindow, 0, 0, cstrDescription, 100 );

        clearInputWindow();
        clearOutputWindow();

        description = cstrDescription;

    } while ( description.length() == 0 );
    noecho();

    return description;
}

//get plan category from a user
std::string View::getCategoryFromUser()
{
    char cstrCategory[26];
    string category;

    echo();
    do
    {
        mvwprintw( outputWindow, 0, 0, "Enter a category." );
        wrefresh( outputWindow );
        mvwgetnstr( inputWindow, 0, 0, cstrCategory, 25 );
        clearInputWindow();
        clearOutputWindow();

        category = cstrCategory;

    } while ( category.length() == 0 );
    noecho();

    return category;
}

//get month from a user to create a monthly plan
std::string View::getMonthFromUser()
{
    echo();
    mvwprintw( outputWindow, 0, 0, "Enter a month." );
    wrefresh( outputWindow );

    char cstrMonth[10];
    mvwgetnstr( inputWindow, 0, 0, cstrMonth, 9 );
    clearInputWindow();
    clearOutputWindow();
    noecho();

    string month = cstrMonth;
    return month;
}

//get year from a user to create an annual plan
string View::getYearFromUser()
{
    char cstrYear[5];
    string sYear;

    echo();
    mvwprintw( outputWindow, 0, 0, "Enter a year." );
    wrefresh( outputWindow );
    mvwgetnstr( inputWindow, 0, 0, cstrYear, 4 );
    clearInputWindow();
    clearOutputWindow();
    noecho();

    string year = cstrYear;
    return year;
}

//get date from a user to create a daily or weekly plan
std::array < std::string, 3 > View::getDateFromUser()
{
    char cstrDay[3];
    char cstrMonth[3];
    char cstrYear[5];

    date d;

    echo();
    mvwprintw( outputWindow, 0, 0, "Enter a date." );
    mvwprintw( outputWindow, 1, 0, "Day:" );
    wrefresh( outputWindow );
    mvwgetnstr( inputWindow, 0, 0, cstrDay, 2 );
    clearInputWindow();
    clearOutputWindow();

    mvwprintw( outputWindow, 0, 0, "Enter a date." );
    mvwprintw( outputWindow, 1, 0, "Month:" );
    wrefresh( outputWindow );
    mvwgetnstr( inputWindow, 0, 0, cstrMonth, 2 );
    clearInputWindow();
    clearOutputWindow();

    mvwprintw( outputWindow, 0, 0, "Enter a date." );
    mvwprintw( outputWindow, 1, 0, "Year:" );
    wrefresh( outputWindow );
    mvwgetnstr( inputWindow, 0, 0, cstrYear, 4 );
    clearInputWindow();
    clearOutputWindow();

    string sDay = cstrDay;
    string sMonth = cstrMonth;
    string sYear = cstrYear;

    return array < string, 3 > {sDay, sMonth, sYear};
}

//choose menu option
char View::getMenuOptionFromUser() const
{
    //enable using special keyboard keys (e.g. arrows)
    keypad( menuWindow, true );
    char ch;
    const int numberOfMenuOptions = 12;
    char menuOptions[numberOfMenuOptions] = {'d', 'w', 'm', 'y', 'a', 'e', 'o', 'n', 'u', 'r', 'k', 'q'};
    int keyPressedByUser;
    int highlightedMenuOption = 0;
    int numberOfOptionsInOneMenuRow = 4;
    int row = 1; //first row in menu window that contains options not headers
    int columnWidth = 28; //number of columns that should be highlighted
    int optionWidth = 40; //number of columns between first char of one menu option to the first char of next option in the same row

    while ( true )
    {
        //find menu option that should be highlighted
        for ( int i = 0; i < numberOfMenuOptions; ++i )
        {
            if ( i == highlightedMenuOption )
            {
                //menu has 3 rows with 4 options in each row; find place that should be highlighted
                if ( i < numberOfOptionsInOneMenuRow )
                {
                    mvwchgat( menuWindow, row, i % numberOfOptionsInOneMenuRow * optionWidth, columnWidth,
                              A_REVERSE, 0, NULL );
                }
                else if ( i >= numberOfOptionsInOneMenuRow && i < 2 * numberOfOptionsInOneMenuRow )
                {
                    mvwchgat( menuWindow, row + 1, i % numberOfOptionsInOneMenuRow * optionWidth, columnWidth,
                              A_REVERSE, 0, NULL );
                }
                else
                {
                    mvwchgat( menuWindow, row + 2, i % numberOfOptionsInOneMenuRow * optionWidth, columnWidth,
                              A_REVERSE, 0, NULL );
                }
            }
        }
        //check whether a user pressed an arrow or enter key
        keyPressedByUser = wgetch( menuWindow );
        switch ( keyPressedByUser )
        {
        case KEY_LEFT:
            highlightedMenuOption--;
            if ( highlightedMenuOption == -1 )
            {
                highlightedMenuOption = 0;
            }
            break;
        case KEY_RIGHT:
            highlightedMenuOption++;
            if ( highlightedMenuOption == numberOfMenuOptions )
            {
                highlightedMenuOption = numberOfMenuOptions - 1;
            }
            break;
        default:
            break;
        }

        //if user pressed enter get the highlighted menu option
        if ( keyPressedByUser == ( int )'\n' )
        {
            ch = menuOptions[highlightedMenuOption];
            break;
        }

        //before next change of highlighted option, clear highlight of a previous one
        clearHighlightInMenu();
    }

    //after choosing a menu option clear a highlight
    clearHighlightInMenu();

    keypad( menuWindow, false );
    return ch;
}

//clear highlight in the entire menu
void View::clearHighlightInMenu() const
{
    int numberOfRowsInMenu = 3;
    int firstColumn = 0;
    for ( int i = 0; i < numberOfRowsInMenu; ++i )
    {
        mvwchgat( menuWindow, i + 1, firstColumn, -1, A_NORMAL, 0, NULL );
    }
}

int View::choosePlanToChange( int numberOfPlans )
{
    //enable using special keyboard keys (e.g. arrows)
    keypad( plansWindow, true );
    int keyPressedByUser;
    int highlightedPlan = 0;
    int firstRow = 3; //first row in plans window that contains plans not headers
    int columnWidth = 141; //number of columns that should be highlighted
    int firstColumn = 0; //first column in a row

    while ( true )
    {
        //find plan that should be highlighted
        for ( int i = 0; i < numberOfPlans; ++i )
        {
            if ( i == highlightedPlan )
            {
                mvwchgat( plansWindow, highlightedPlan + firstRow, firstColumn, columnWidth, A_REVERSE, 0, NULL );
            }
        }
        keyPressedByUser = wgetch( plansWindow );

        //check whether a user pressed an arrow or enter key
        switch ( keyPressedByUser )
        {
        case KEY_UP:
            highlightedPlan--;
            if ( highlightedPlan == -1 )
            {
                highlightedPlan = 0;

            }
            //before highlight the next plan, uncheck previous selected one
            mvwchgat( plansWindow, firstRow + highlightedPlan + 1, firstColumn, -1, A_NORMAL, 0, NULL );
            break;
        case KEY_DOWN:
            highlightedPlan++;
            if ( highlightedPlan == numberOfPlans )
            {
                highlightedPlan = numberOfPlans - 1;

            }
            //before highlight the next plan, uncheck previous selected one
            mvwchgat( plansWindow, firstRow + highlightedPlan - 1, firstColumn, -1, A_NORMAL, 0, NULL );
            break;
        default:
            break;
        }

        //if user pressed enter get the highlighted menu option
        if ( keyPressedByUser == ( int )'\n' )
        {
            break;
        }
    }

    //clear highlight in the entire plans window
    for ( int j = 0; j < numberOfPlans; ++j )
    {
        mvwchgat( plansWindow, firstRow + j, firstColumn, -1, A_NORMAL, 0, NULL );
    }

    keypad( plansWindow, false );
    return highlightedPlan;
}

