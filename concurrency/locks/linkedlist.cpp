#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>

class ListNode {
    public:
    int val;
    ListNode *next;
    std::mutex mux;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class LinkedList {
    public:
    ListNode *head;
    LinkedList() : head(nullptr) {}
    
    void addNode(int val) {
        ListNode *toAdd = new ListNode(val);
        if (head == nullptr) {
            head = toAdd;
            return;
        }

        ListNode *curr = head;
        while (true) {
            curr->mux.lock();
            if (curr->next == nullptr) {
                curr->next = toAdd;
                break;
            }
            curr->mux.unlock();
            curr = curr->next;
        }
        curr->mux.unlock();
    }

    ~LinkedList() {
        ListNode *curr = head;
        while (curr != nullptr) {
            ListNode *next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
    }

    void printList() {
        ListNode *curr = head;
        while (curr != nullptr) {
            std::cout << curr->val << " ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    int size() {
        int count = 0;
        ListNode *curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
};

void addNodes(LinkedList *list) {
    for (int i = 0; i < 1000; i++) {
        list->addNode(i);
    }
}

int main() {
    LinkedList *list = new LinkedList();

    std::thread t1(addNodes, list);
    std::thread t2(addNodes, list);
    t1.join();
    t2.join();

    list->printList();
    std::cout << "List size: " << list->size() << std::endl;
    delete list;
    return 0;
}