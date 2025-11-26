#include "Event.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;
using namespace std::chrono;

int Event::idCounter = 0;

// 解析字符串成 minutes
minutes Event::parseTime(string_view time) {
    int h = stoi(string(time.substr(0, 2)));
    int m = stoi(string(time.substr(3, 2)));
    return hours(h) + minutes(m);
}

Event::Event(string name, string startTimeStr, string endTimeStr) : name(name) {
    id = ++idCounter;
    startTime = parseTime(startTimeStr);
    endTime = parseTime(endTimeStr);
}

int Event::getId() const {
    return id;
}

string Event::getName() const {
    return name;
}

minutes Event::getStartTime() const {
    return startTime;
}

minutes Event::getEndTime() const {
    return endTime;
}

string Event::formatTime(minutes time) const {
    int h = time.count() / 60;
    int m = time.count() % 60;
    stringstream ss;
    ss << setfill('0') << setw(2) << h << ":" << setfill('0') << setw(2) << m;
    return ss.str();
}

void Event::print() const {
    cout << "ID: " << id 
         << " | Name: " << name 
         << " | Start: " << formatTime(startTime) 
         << " | End: " << formatTime(endTime) 
         << " | Duration: " << getDuration() << " min" << endl;
}

bool Event::operator<(const Event& other) const {
    return startTime < other.startTime;
}

void Event::save(ofstream& out) const {
    out.write((char*)&id, sizeof(id));
    size_t nameLen = name.size();
    out.write((char*)&nameLen, sizeof(nameLen));
    out.write(name.c_str(), nameLen);
    long long startCount = startTime.count();
    out.write((char*)&startCount, sizeof(startCount));
    long long endCount = endTime.count();
    out.write((char*)&endCount, sizeof(endCount));
}

void Event::load(ifstream& in) {
    in.read((char*)&id, sizeof(id));
    if (id > idCounter) idCounter = id;

    size_t nameLen;
    in.read((char*)&nameLen, sizeof(nameLen));
    name.resize(nameLen);
    in.read(&name[0], nameLen);

    long long startCount;
    in.read((char*)&startCount, sizeof(startCount));
    startTime = minutes(startCount);

    long long endCount;
    in.read((char*)&endCount, sizeof(endCount));
    endTime = minutes(endCount);
}

// 获得持续时间
int Event::getDuration() const {
    return (endTime - startTime).count();
}

void Event::setName(string name) {
    this->name = name;
}

void Event::setStartTime(string startTimeStr) {
    this->startTime = parseTime(startTimeStr);
}

void Event::setEndTime(string endTimeStr) {
    this->endTime = parseTime(endTimeStr);
}