#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <chrono>
#include "Day.h"
#include "Event.h"
#include "LinkList.h"

namespace TimeReport {

    using namespace std;
    using namespace std::chrono;

    //  1. 配置区域：颜色与样式 
    struct Style {
        string color;
        string label;
    };

    inline Style getStyleByCategory(string type) {
        if (type == "STUDY") return { "#d43307ff", "学习" }; // 蓝色
        if (type == "WORK")  return { "#4863eaff", "工作" }; // 紫色
        if (type == "LIFE")  return { "#10B981", "生活" }; // 绿色
        if (type == "ENT")   return { "#F59E0B", "休闲" }; // 黄色
        return { "#94A3B8", "其他" };                        // 灰色
    }

    //  2. 分类推断 
    inline string inferCategory(const string& name) {
        string n = name; 
        if (n.find("学") != string::npos || n.find("习") != string::npos || n.find("书") != string::npos || n.find("课") != string::npos || n.find("C++") != string::npos || n.find("算法") != string::npos) return "STUDY";
        if (n.find("会") != string::npos || n.find("工") != string::npos || n.find("代") != string::npos || n.find("码") != string::npos) return "WORK";
        if (n.find("饭") != string::npos || n.find("睡") != string::npos || n.find("休") != string::npos || n.find("跑") != string::npos || n.find("洗") != string::npos) return "LIFE";
        if (n.find("游") != string::npos || n.find("玩") != string::npos || n.find("视") != string::npos || n.find("网") != string::npos) return "ENT";
        return "OTHER";
    }

    //  3. 辅助计算 
    inline string formatTime(minutes t) {
        int h = t.count() / 60;
        int m = t.count() % 60;
        char buf[10];
        sprintf(buf, "%02d:%02d", h, m);
        return string(buf);
    }

    inline string formatDuration(int minutes) {
        int h = minutes / 60;
        int m = minutes % 60;
        string res = "";
        if (h > 0) res += to_string(h) + "h ";
        res += to_string(m) + "m";
        return res;
    }

    //  4. SVG 绘制类 
    class SVGWriter {
        ofstream file;
    public:
        SVGWriter(const string& filename, int width, int height) {
            file.open(filename);
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
            file << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
            // 白色背景
            file << "<rect width=\"100%\" height=\"100%\" fill=\"#F8F9FA\" />\n";
        }

        ~SVGWriter() {
            file << "</svg>";
            file.close();
        }

        void drawText(int x, int y, const string& text, int fontSize, string color, string weight = "normal") {
            file << "<text x=\"" << x << "\" y=\"" << y << "\" font-family=\"Microsoft YaHei, SimHei, sans-serif\" font-size=\"" << fontSize 
                 << "\" fill=\"" << color << "\" font-weight=\"" << weight << "\">" << text << "</text>\n";
        }

        void drawRect(int x, int y, int w, int h, string color, int rx = 0) {
            file << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << w << "\" height=\"" << h 
                 << "\" fill=\"" << color << "\" rx=\"" << rx << "\" />\n";
        }

        void drawCircle(int cx, int cy, int r, string strokeColor, int strokeWidth, double dashArray, double dashOffset) {
            file << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << r << "\" fill=\"none\" stroke=\"" << strokeColor 
                 << "\" stroke-width=\"" << strokeWidth << "\" stroke-dasharray=\"" << dashArray << " 1000\" stroke-dashoffset=\"" << dashOffset 
                 << "\" transform=\"rotate(-90 " << cx << " " << cy << ")\" />\n";
        }
    };

    //  5. 生成函数 
    inline void generateDailyReport(const Day& day, string filename = "DailyReport.svg") {
        
        struct EventData {
            string name;
            minutes start;
            minutes end;
            int duration;
            string category;
        };
        vector<EventData> events;
        map<string, int> categoryStats;
        int totalMinutes = 0;

        auto p = day.getEventList().getHead();
        while (p) {
            const Event& e = p->data;
            int dur = e.getDuration();
            string cat = inferCategory(e.getName());
            
            events.push_back({e.getName(), e.getStartTime(), e.getEndTime(), dur, cat});
            categoryStats[cat] += dur;
            totalMinutes += dur;
            p = p->next;
        }

        // 绘图
        int height = max(600, 250 + (int)events.size() * 80);
        SVGWriter svg(filename, 800, height);

        // 标题
        svg.drawText(40, 50, "TimeManager Daily Report", 24, "#1E293B", "bold");
        string dateStr = day.getDate();
        svg.drawText(40, 80, dateStr, 14, "#64748B");

        // 左侧：圆环图
        int cx = 200, cy = 250, r = 80;
        // 底圈
        svg.drawCircle(cx, cy, r, "#E2E8F0", 15, 1000, 0);
        
        // 数据圈
        double circumference = 2 * 3.1415926 * r;
        double currentOffset = 0;
        
        // 绘制圆环片段
        for (auto const& [cat, minutes] : categoryStats) {
            double percent = (totalMinutes > 0) ? (double)minutes / totalMinutes : 0;
            double dashLen = circumference * percent;
            svg.drawCircle(cx, cy, r, getStyleByCategory(cat).color, 15, dashLen, -currentOffset);
            currentOffset += dashLen;
        }

        // 圆心文字
        svg.drawText(cx - 45, cy + 5, formatDuration(totalMinutes), 20, "#0F172A", "bold");
        svg.drawText(cx - 20, cy + 25, "总计", 12, "#94A3B8");

        // 左侧下方：图例
        int legendY = 400;
        for (auto const& [cat, minutes] : categoryStats) {
            Style s = getStyleByCategory(cat);
            svg.drawRect(120, legendY - 10, 12, 12, s.color, 2);
            svg.drawText(140, legendY, s.label + ": " + formatDuration(minutes), 12, "#475569");
            legendY += 25;
        }

        // 右侧：时间轴列表
        int listX = 400;
        int listY = 120;
        
        svg.drawText(listX, listY, "详细时间轴", 14, "#475569", "bold");
        listY += 30;

        // 竖线背景
        svg.drawRect(listX + 14, listY, 2, height - listY - 50, "#E2E8F0");

        for (const auto& e : events) {
            Style s = getStyleByCategory(e.category);
            
            // 时间点圆圈
            svg.drawCircle(listX + 15, listY + 25, 6, s.color, 4, 1000, 0); 
            
            // 卡片背景
            svg.drawRect(listX + 40, listY, 320, 60, "white", 8);
            // 左边框条
            svg.drawRect(listX + 40, listY, 4, 60, s.color, 0); 
            
            // 文字内容
            string timeRange = formatTime(e.start) + " - " + formatTime(e.end);
            svg.drawText(listX + 55, listY + 20, timeRange, 10, "#94A3B8");
            svg.drawText(listX + 55, listY + 40, e.name, 14, "#1E293B", "bold");
            svg.drawText(listX + 300, listY + 35, formatDuration(e.duration), 12, s.color, "bold");

            listY += 75;
        }

        cout << "成功生成报表: " << filename << endl;
    }
}

#endif