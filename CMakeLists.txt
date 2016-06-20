cmake_minimum_required(VERSION 3.5)
project(demomeni)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c logic.h logic.c mtwist.c mtwist.h display.c display.h playgame.c playgame.h ai.h ai.c timers.h timers.c bot.c)
add_executable(demomeni ${SOURCE_FILES})

target_link_libraries(demomeni pdcurses winmm)