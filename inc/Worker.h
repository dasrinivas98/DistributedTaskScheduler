#ifndef WORKER_H
#define WORKER_H

#include "Task.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

class Worker {
public:
    Worker();
    void start();
    void stop();
    void addTask(const Task& task);

private:
    void workerThread();
    std::queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::thread worker;
    std::atomic<bool> running;
};

#endif // WORKER_H