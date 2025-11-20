#include "Day.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int Day::idCounter = 0;

Day::Day(string data) : data(data) {
    id = ++idCounter;
};

int Day::getId() const {
    return id;
}

string Day::getDate() const {
    return data;
}

void Day::addEvent(Event e) {
    eventList.insertSorted(e);
}

bool Day::removeEventById(int id) {
    return eventList.removeById(id);
}

void Day::print() const {
    cout << "Date: " << data << endl;
    eventList.print();
}

void Day::draw() const {
    cout << "┌────────────────────────────────────────────────────────┐" << endl;
    cout << "│ Schedule for " << left << setw(42) << data << "│" << endl;
    cout << "├───────┬───────┬────────────────────────────────────────┤" << endl;
    cout << "│ Start │ End   │ Event                                  │" << endl;
    cout << "├───────┼───────┼────────────────────────────────────────┤" << endl;
    
    auto node = eventList.getHead();
    while (node != nullptr) {
        Event& e = node->data;
        cout << "│ " << e.formatTime(e.getStartTime()) << " │ " 
             << e.formatTime(e.getEndTime()) << " │ " 
             << left << setw(38) << e.getName() << " │" << endl;
        node = node->next;
    }
    cout << "└───────┴───────┴────────────────────────────────────────┘" << endl;
}

void Day::save(ofstream& out) const {
    out.write((char*)&id, sizeof(id));
    size_t dataLen = data.size();
    out.write((char*)&dataLen, sizeof(dataLen));
    out.write(data.c_str(), dataLen);
    eventList.save(out);
}

void Day::load(ifstream& in) {
    in.read((char*)&id, sizeof(id));
    if (id > idCounter) idCounter = id;

    size_t dataLen;
    in.read((char*)&dataLen, sizeof(dataLen));
    data.resize(dataLen);
    in.read(&data[0], dataLen);
    
    eventList.load(in);
}