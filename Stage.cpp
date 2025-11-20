#include "Stage.h"
#include "ReportGenerator.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

Stage::Stage() {
    string todayDate = getCurrentDateStr();
    
    // 查找是否已有今天的记录
    currentDay = nullptr;
    auto head = dayList.getHead();
    while (head != nullptr) {
        if (head->data.getDate() == todayDate) {
            currentDay = &(head->data);
            break;
        }
        head = head->next;
    }

    if (currentDay == nullptr) {
        Day newDay(todayDate);
        dayList.add(newDay);
        head = dayList.getHead();
        while (head != nullptr) {
            if (head->data.getDate() == todayDate) {
                currentDay = &(head->data);
                break;
            }
            head = head->next;
        }
    }
    
    isRunning = false;
}

Stage::~Stage() {
}

string Stage::getCurrentDateStr() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

string Stage::getCurrentTimeStr() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%H:%M");
    return oss.str();
}

void Stage::start(string name) {
    if (isRunning) {
        cout << "当前已有任务在进行中: " << currentTaskName << endl;
        return;
    }
    currentTaskName = name;
    startTimeStr = getCurrentTimeStr();
    isRunning = true;
    cout << "任务 [" << name << "] 开始于 " << startTimeStr << endl;
}

void Stage::end() {
    if (!isRunning) {
        cout << "当前没有正在进行的任务" << endl;
        return;
    }
    string endTimeStr = getCurrentTimeStr();
    Event newEvent(currentTaskName, startTimeStr, endTimeStr);
    currentDay->addEvent(newEvent);
    
    cout << "任务 [" << currentTaskName << "] 结束于 " << endTimeStr << endl;
    cout << "持续时间: " << newEvent.getDuration() << " 分钟" << endl;
    
    isRunning = false;
}

void Stage::add(string date, string name, string startTime, string endTime) {
    Day* targetDay = nullptr;
    auto head = dayList.getHead();
    while (head != nullptr) {
        if (head->data.getDate() == date) {
            targetDay = &(head->data);
            break;
        }
        head = head->next;
    }

    if (targetDay == nullptr) {
        Day newDay(date);
        dayList.add(newDay);
        
        head = dayList.getHead();
        while (head != nullptr) {
            if (head->data.getDate() == date) {
                targetDay = &(head->data);
                break;
            }
            head = head->next;
        }
    }

    Event newEvent(name, startTime, endTime);
    targetDay->addEvent(newEvent);
    cout << "成功添加事件: " << name << " 到 " << date << endl;
}

void Stage::deleteEvent(int id) {
    if (currentDay != nullptr) {
        if (currentDay->removeEventById(id)) {
            cout << "成功删除 ID 为 " << id << " 的事件" << endl;
        } else {
            cout << "未找到 ID 为 " << id << " 的事件" << endl;
        }
    }
}

void Stage::draw(string date) {
    Day* targetDay = nullptr;
    auto head = dayList.getHead();
    while (head != nullptr) {
        if (head->data.getDate() == date) {
            targetDay = &(head->data);
            break;
        }
        head = head->next;
    }

    if (targetDay == nullptr) {
        Day newDay(date);
        dayList.add(newDay);
        // 重新查找获取指针
        head = dayList.getHead();
        while (head != nullptr) {
            if (head->data.getDate() == date) {
                targetDay = &(head->data);
                break;
            }
            head = head->next;
        }
        cout << "已创建日期为 " << date << " 的新日程表" << endl;
    }

    targetDay->draw();
}

void Stage::generateReport(string date) {
    Day* targetDay = nullptr;
    auto head = dayList.getHead();
    while (head != nullptr) {
        if (head->data.getDate() == date) {
            targetDay = &(head->data);
            break;
        }
        head = head->next;
    }

    if (targetDay == nullptr) {
        cout << "未找到日期为 " << date << " 的记录，无法生成报表" << endl;
        return;
    }

    string filename = "report_" + date + ".svg";
    TimeReport::generateDailyReport(*targetDay, filename);
}

void Stage::printLog() {
    if (currentDay != nullptr) {
        currentDay->print();
    }
}

void Stage::printGlobal() {
    dayList.print();
}

void Stage::saveToFile(string filename) {
    ofstream out(filename, ios::binary);
    if (!out) return;
    dayList.save(out);
    out.close();
}

void Stage::loadFromFile(string filename) {
    ifstream in(filename, ios::binary);
    if (!in) return;
    dayList.load(in);
    in.close();
    
    // 恢复 currentDay 指针
    string todayDate = getCurrentDateStr();
    currentDay = nullptr;
    auto head = dayList.getHead();
    while (head != nullptr) {
        if (head->data.getDate() == todayDate) {
            currentDay = &(head->data);
            break;
        }
        head = head->next;
    }
    
    if (currentDay == nullptr) {
        Day newDay(todayDate);
        dayList.add(newDay);
        // 重新获取
        head = dayList.getHead();
        while (head != nullptr) {
            if (head->data.getDate() == todayDate) {
                currentDay = &(head->data);
                break;
            }
            head = head->next;
        }
    }
}
