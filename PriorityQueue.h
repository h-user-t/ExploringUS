// PriorityQueue.h
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>

struct Node {
    int id;
    int distance;

    Node(int id, double distance);

    bool operator<(const Node& other) const;
};

class PriorityQueue {
private:
    std::vector<Node> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    PriorityQueue();

    bool isEmpty() const;
    int size() const;

    void push(int id, double distance);
    Node top() const;
    void pop();
};

#endif // PRIORITYQUEUE_H
