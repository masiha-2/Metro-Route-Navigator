#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <stdexcept>
#include <utility>

/**
 * Custom Min-Heap implementation built from scratch (without std::priority_queue)
 * Used as the priority queue engine for Dijkstra's Shortest Path Algorithm.
 *
 * Time Complexities:
 *   push()  : O(log n)
 *   pop()   : O(log n)
 *   top()   : O(1)
 *   empty() : O(1)
 */
template <typename T>
class MinHeap {
private:
    std::vector<T> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int index) {
        while (index > 0 && heap[index] < heap[parent(index)]) {
            std::swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int n = static_cast<int>(heap.size());
        while (true) {
            int smallest = index;
            int left = leftChild(index);
            int right = rightChild(index);

            if (left < n && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < n && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    void pop() {
        if (heap.empty()) {
            throw std::underflow_error("MinHeap is empty");
        }
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    const T& top() const {
        if (heap.empty()) {
            throw std::underflow_error("MinHeap is empty");
        }
        return heap[0];
    }

    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }
};

#endif // MINHEAP_H
