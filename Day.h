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
    Event* getEventById(int id);
    bool updateEvent(int id, std::string newName, std::string newStart, std::string newEnd);
    bool removeEventById(int id);
    bool isEmpty() const;
    void print() const;
    void draw() const;
    bool operator<(const Day& other) const;

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

    const LinkList<Event>& getEventList() const;
};