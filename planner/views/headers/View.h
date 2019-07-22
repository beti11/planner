#ifndef PLANNER_VIEW_H
#define PLANNER_VIEW_H

#include <ncurses.h>
#include <memory>
#include <vector>

class Plan;

class View
{
public:
    View();
    virtual ~View() = default;
    void createDailyPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, boost::gregorian::date date );
    void createWeeklyPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, boost::gregorian::date date );
    void createMonthlyPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, const std::string &month, int year );
    void createAnnualPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans, int year );
    void createMenuWindow();
    void createOutputWindow();
    void createInputWindow();
    void clearInputWindow();
    void clearOutputWindow();
    std::string getDescriptionFromUser();
    std::string getCategoryFromUser();
    std::string getMonthFromUser();
    std::string getYearFromUser();
    std::array < std::string, 3 > getDateFromUser();
    char getMenuOptionFromUser() const;
    int choosePlanToChange( int numberOfPlans );
    void exit();
private:
    WINDOW *plansWindow;
    WINDOW *menuWindow;
    WINDOW *outputWindow;
    WINDOW *inputWindow;
    void createPlansWindow( const std::vector < std::shared_ptr < Plan>> &plans );
    void clearHighlightInMenu() const;
    static std::string createHeaderLine();
    static const int lineWidth = 145;
    static const int rows = 45;
};

#endif //PLANNER_VIEW_H
