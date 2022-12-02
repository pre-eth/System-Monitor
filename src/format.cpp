#include "format.h"

std::string Format::ElapsedTime(long seconds) { 
    std::ostringstream date;

    int hours = std::floor(seconds / 3600);
    seconds %= 3600;
    
    int min = std::floor(seconds / 60);
    seconds %= 60;
    
    date << std::setw(2) << std::setfill('0') << hours;
    date << ":" << std::setw(2) << min;
    date << ":" << std::setw(2) << seconds;

    return date.str();
 }