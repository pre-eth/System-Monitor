#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
    void Display(System& system, int n = 10);
    void DisplaySystem(System& system, WINDOW* window);
    void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
    std::string ProgressBar(float percent);
    int const pid_column{2};
    int const user_column{9};
    int const cpu_column{18};
    int const ram_column{26};
    int const time_column{35};
    int const command_column{46};
};  

#endif