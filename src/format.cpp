#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    std::ostringstream date;

    int hours = std::floor(seconds / 3600);
    seconds %= 3600;
    int min = std::floor(seconds / 60);
    seconds %= 60;
    
    date << std::setw(2) << std::setfill('0') << hours;
    date << ":" << std::setw(2) << min;
    date << ":" << seconds;

    return date.str();
 }