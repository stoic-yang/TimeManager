#include <iostream>
#include <cstdlib>
#include "Day.h"
#include "Event.h"
#include "Stage.h"
#include "LinkList.h"
#include "Utils.h"
#include "ReportGenerator.h"

bool isCounting = false;
Stage* stage = nullptr;

int main() {
    system("chcp 65001");
    init();
    stage = new Stage();
    stage->loadFromFile(".TimeManager");

    while (true) {
        std::string op;
        std::cin >> op;
        if (op == "exit") {
            stage->saveToFile(".TimeManager");
            std::cout << "程序终止" << std::endl;
            break;
        }
        // help 指令
        else if (op == "help") {
            help();
        }
        // start 指令
        else if (op == "start") {
            std::cout << "请输入任务名称:" << std::endl;
            std::string name;
            std::cin >> name;
            stage->start(name);
        }
        else if (op == "end") {
            stage->end();
        }
        else if (op == "log") {
            stage->printLog();
        }
        else if (op == "global") {
            stage->printGlobal();
        }
        else if (op == "add") {
            std::string date, name, startTime, endTime;
            std::cin >> date >> name >> startTime >> endTime;
            stage->add(date, name, startTime, endTime);
        }
        else if (op == "del") {
            int id;
            std::cin >> id;
            stage->deleteEvent(id);
        }
        else if (op == "draw") {
            std::string date;
            std::cin >> date;
            stage->draw(date);
        }
        else if (op == "report") {
            std::string date;
            std::cin >> date;
            stage->generateReport(date);
        }
        else {
            std::cout << "请输入合法的指令： " << std::endl;
        }
    }
}