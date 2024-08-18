#include "Worker.h"
#include <iostream>

Worker::Worker() : running(false) {}

void Worker::start() {
    running = true;
    worker = std::thread(&Worker::workerThread, this);
}

void Worker::stop() {
    running = false;
    condition.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
}

void Worker::addTask(const Task& task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(task);
    }
    condition.notify_one();
}

void Worker::workerThread() {
    while (running) {
        Task task(0, "");
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !taskQueue.empty() || !running; });
            if (!running && taskQueue.empty()) {
                return;
            }
            task = taskQueue.front();
            taskQueue.pop();
        }
        // Process the task
        std::cout << "Processing task ID: " << task.getId() << " with description: " << task.getDescription() << std::endl;
    }
}