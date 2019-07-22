#ifndef PLANNER_PLAN_H
#define PLANNER_PLAN_H

#include <string>
#include <ostream>
#include <boost/date_time/gregorian/greg_date.hpp>

class Plan
{
public:
    Plan( std::string description, std::string category );
    Plan( int id, std::string description, std::string category, bool done );
    virtual ~Plan() = default;
    std::string print() const;
    void setDescription( const std::string &description );
    void setCategory( const std::string &category );
    void setDone( bool done );
    int getId() const;
    const std::string &getDescription() const;
    const std::string &getCategory() const;
    bool isDone() const;

private:
    int id;
    std::string description;
    std::string category;
    bool done = false;
};

#endif //PLANNER_PLAN_H
