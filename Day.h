#pragma once
#include <string>
#include <fstream>
#include "Event.h"
#include "LinkList.h"

class Day {
private:
    int id;
    static int idCounter;
    LinkList<Event> eventList;
    std::string data; 
public:
    Day() = default;
    Day(std::string data);
    int getId() const;
    std::string getDate() const;
    void addEvent(Event e);
    bool removeEventById(int id);
    void print() const;
    void draw() const;
    
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
};