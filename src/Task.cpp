#include "Task.h"

Task::Task(int id, const std::string& description)
    : id(id), description(description) {}

int Task::getId() const { return id; }
std::string Task::getDescription() const { return description; }