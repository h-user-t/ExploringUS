// PriorityQueue.cpp
#include "PriorityQueue.h"
#include <iostream>

using namespace std;

Node::Node(int id, double distance) : id(id), distance(distance) {}

bool Node::operator<(const Node& other) const {
    return distance < other.distance;
}

PriorityQueue::PriorityQueue() {}

bool PriorityQueue::isEmpty() const {
    return heap.empty();
}

int PriorityQueue::size() const {
    return heap.size();
}

void PriorityQueue::push(int id, double distance) {
    Node newNode(id, distance);
    heap.push_back(newNode);
    heapifyUp(heap.size() - 1);
}

Node PriorityQueue::top() const {
    if (isEmpty()) {
        throw runtime_error("Priority Queue is empty");
    }
    return heap[0];
}


void PriorityQueue::pop() {
    if (isEmpty()) {
        throw runtime_error("Priority Queue is empty");
    }
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        heapifyDown(0);
    }
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heap[index] < heap[parentIndex]) {
            std::swap(heap[parentIndex], heap[index]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void PriorityQueue::heapifyDown(int index) {
    int smallest = index;
    int leftChild = (2 * index) + 1;
    int rightChild = (2 * index) + 2;

    if (leftChild < size() && heap[leftChild] < heap[smallest]) {
        smallest = leftChild;
    }

    if (rightChild < size() && heap[rightChild] < heap[smallest]) {
        smallest = rightChild;
    }

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}
