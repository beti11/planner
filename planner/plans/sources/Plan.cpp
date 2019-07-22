#include <iostream>
#include <boost/date_time/gregorian/parsers.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "../headers/Plan.h"

using namespace boost::gregorian;
using namespace std;

Plan::Plan( string description, string category )
        : description {std::move( description )}, category {std::move( category )}
{
}

Plan::Plan( int id, string description, string category, bool done )
        : id {id}, description {std::move( description )}, category {std::move( category )}, done {done}
{
}

string Plan::print() const
{
    int idLength = 5;
    int descLength = 102;
    int catLength = 28;
    int doneLength = 5;

    stringstream buffer;
    buffer
            << setw( idLength )
            << left
            << id
            << setw( descLength )
            << left
            << description
            << setw( catLength )
            << left
            << category
            << setw( doneLength )
            << left
            << boolalpha
            << done;
    return buffer.str();
}

void Plan::setDescription( const string &description )
{
    Plan::description = description;
}

void Plan::setCategory( const std::string &category )
{
    Plan::category = category;
}

int Plan::getId() const
{
    return id;
}

const std::string &Plan::getDescription() const
{
    return description;
}

const std::string &Plan::getCategory() const
{
    return category;
}

void Plan::setDone( bool done )
{
    Plan::done = done;
}

bool Plan::isDone() const
{
    return done;
}
