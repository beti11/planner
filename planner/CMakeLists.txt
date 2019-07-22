
cmake_minimum_required(VERSION 3.10)
project(planner)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")

add_executable(planner plans/sources/Plan.cpp plans/headers/Plan.h planServices/sources/PlanService.cpp planServices/headers/PlanService.h facade/sources/Facade.cpp facade/headers/Facade.h sqlService/sources/SQLService.cpp sqlService/headers/SQLService.h views/sources/View.cpp views/headers/View.h plans/sources/DailyPlan.cpp plans/headers/DailyPlan.h plans/sources/WeeklyPlan.cpp plans/headers/WeeklyPlan.h plans/sources/MonthlyPlan.cpp plans/headers/MonthlyPlan.h plans/sources/AnnualPlan.cpp plans/headers/AnnualPlan.h)

#boost
find_package(Boost)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

if (Boost_FOUND)
    set(BOOST_INCLUDE_DIRS "/usr/include")
    set(BOOST_LIBRARY_DIRS "/usr/lib")
    find_package(Boost REQUIRED date_time)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(planner ${Boost_LIBRARIES})
endif ()
#end boost

#ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(planner ncurses)
#end ncurses

#cppconn
target_link_libraries(planner mysqlcppconn)
