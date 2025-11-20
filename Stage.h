#pragma once
#include "Day.h"
#include <string>

class Stage {
private:
    LinkList<Day> dayList;
    Day* currentDay;
    std::string currentTaskName;
    std::string startTimeStr;
    bool isRunning;

    std::string getCurrentTimeStr();
    std::string getCurrentDateStr();

public:
    Stage();
    ~Stage();
    void start(std::string name);
    void end();
    void add(std::string date, std::string name, std::string startTime, std::string endTime);
    void deleteEvent(int id);
    void draw(std::string date);
    void printLog();
    void printGlobal();
    
    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);
};