#include "Utils.h"
#include <iostream>

using namespace std;

void help() {
    cout << "可用指令:" << endl;
    cout << "  start <name>                           : 开始一个新事件" << endl;
    cout << "  end                                    : 结束当前事件" << endl;
    cout << "  log                                    : 打印今天的日志" << endl;
    cout << "  global                                 : 打印所有日志" << endl;
    cout << "  add <date> <name> <start> <end>        : 手动添加事件" << endl;
    cout << "  update <id> <date> <name> <start> <end>: 修改指定ID的事件" << endl;
    cout << "  del <id>                               : 删除指定ID的事件" << endl;
    cout << "  draw <date>                            : 绘制日程表 (格式: YYYY-MM-DD)" << endl;
    cout << "  report <date>                          : 生成SVG报表" << endl;
    cout << "  help                                   : 显示帮助信息" << endl;
    cout << "  exit                                   : 退出程序" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
}

void init() {
    cout << "==========================================================================" << endl;
    cout << "                           时间日志启动 !              " << endl;
    cout << "==========================================================================" << endl;
    help();
}
