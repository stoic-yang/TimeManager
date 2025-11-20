#pragma once
#include <iostream>
#include <fstream>

template <typename T>
class LinkList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* head;
    Node* cur;

public:
    LinkList() {
        head = nullptr;
        cur = nullptr;
    }

    // 拷贝构造函数
    LinkList(const LinkList& other) {
        head = nullptr;
        cur = nullptr;
        Node* p = other.head;
        while (p != nullptr) {
            add(p->data);
            p = p->next;
        }
    }

    // 赋值运算符重载
    LinkList& operator=(const LinkList& other) {
        if (this == &other) return *this;
        
        // 清空当前链表
        Node* p = head;
        while (p != nullptr) {
            Node* temp = p;
            p = p->next;
            delete temp;
        }
        head = nullptr;
        cur = nullptr;

        // 拷贝新数据
        p = other.head;
        while (p != nullptr) {
            add(p->data);
            p = p->next;
        }
        return *this;
    }

    Node* getHead() const {
        return head;
    }

    // 链表add操作
    void add(T val) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
            cur = newNode;
        } else {
            cur->next = newNode;
            cur = newNode;
        }
    }

    void insertSorted(T val) {
        Node* newNode = new Node(val);

        if (head == nullptr || val < head->data) {
            newNode->next = head;
            head = newNode;
            if (cur == nullptr) cur = newNode; 
            if (newNode->next == nullptr) cur = newNode; 
            return;
        }

        Node* p = head;
        while (p->next != nullptr && !(val < p->next->data)) {
            p = p->next;
        }
        
        newNode->next = p->next;
        p->next = newNode;

        if (newNode->next == nullptr) {
            cur = newNode;
        }
    }

    // 链表remove操作
    bool remove(const T& val) {
        if (head == nullptr) return false;
        
        if (head->data == val) {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) cur = nullptr;
            delete temp;
            return true;
        }
        
        Node* p = head;
        while (p->next != nullptr) {
            if (p->next->data == val) {
                Node* temp = p->next;
                p->next = temp->next;
                if (temp == cur) cur = p;
                delete temp;
                return true;
            }
            p = p->next;
        }
        return false;
    }

    // 根据ID删除节点
    template <typename K>
    bool removeById(K id) {
        if (head == nullptr) return false;
        
        if (head->data.getId() == id) {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) cur = nullptr;
            delete temp;
            return true;
        }
        
        Node* p = head;
        while (p->next != nullptr) {
            if (p->next->data.getId() == id) {
                Node* temp = p->next;
                p->next = temp->next;
                if (temp == cur) cur = p;
                delete temp;
                return true;
            }
            p = p->next;
        }
        return false;
    }

    void print() const {
        Node* p = head;
        while (p != nullptr) {
            p->data.print();
            p = p->next;
        }
    }

    void save(std::ofstream& out) const {
        int count = 0;
        Node* p = head;
        while (p) { count++; p = p->next; }
        out.write((char*)&count, sizeof(count));

        p = head;
        while (p) {
            p->data.save(out);
            p = p->next;
        }
    }

    void load(std::ifstream& in) {
        // 清空现有
        while(head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        cur = nullptr;

        int count;
        in.read((char*)&count, sizeof(count));
        for (int i = 0; i < count; ++i) {
            T val; 
            val.load(in);
            add(val); 
        }
    }

    ~LinkList() {
        Node* p = head;
        while (p != nullptr) {
            Node* temp = p;
            p = p->next;
            delete temp;
        }
    }
};