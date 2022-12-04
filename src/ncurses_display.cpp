#include "format.h"
#include "ncurses_display.h"
#include "system.h"

using std::string, std::to_string;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
string NCursesDisplay::ProgressBar(float percent) {
  string result{"0% "};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i)
    result += i <= bars ? '|' : ' ';

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem().substr(0, 15)).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel().substr(0, 21)).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Utilization()).c_str());
  mvwprintw(window, ++row, 2, "Memory: ");
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  system.RefreshProcessInfo();
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  system.RefreshUpTime();
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime()) + 
            "\tIdle Time: " + Format::ElapsedTime(system.IdleTime())).c_str());

}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  for (int i{0}; i < n; i++) {
    // Clear the line
    mvwprintw(window, ++row, pid_column, (string(window->_maxx-2, ' ').c_str()));
    mvwprintw(window, row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization();
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, to_string(processes[i].Ram()).c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color
  init_color(COLOR_GREEN, 300, 850, 75);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(9, x_max, 0, 0);
  wattron(system_window, COLOR_PAIR(1));
  WINDOW* process_window =
    newwin(3 + n, x_max, system_window->_maxy + 1, 0);
  wattron(process_window, COLOR_PAIR(1));
   
  while (1) {
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  endwin();
}
