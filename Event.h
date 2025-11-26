#pragma once
#include <string>
#include <string_view>
#include <chrono>
#include <fstream>

class Event {
private:
    int id;
    static int idCounter;
    std::string name;
    std::chrono::minutes startTime;
    std::chrono::minutes endTime;
    std::chrono::minutes parseTime(std::string_view time);

public:
    Event() = default;
    Event(std::string name, std::string startTimeStr, std::string endTimeStr);
    int getDuration() const;
    int getId() const;
    std::string getName() const;
    std::chrono::minutes getStartTime() const;
    std::chrono::minutes getEndTime() const;
    void setName(std::string name);
    void setStartTime(std::string startTimeStr);
    void setEndTime(std::string endTimeStr);
    void print() const;
    std::string formatTime(std::chrono::minutes time) const;
    bool operator<(const Event& other) const;

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
};
