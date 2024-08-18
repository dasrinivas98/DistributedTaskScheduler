#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
public:
    Task(int id, const std::string& description);
    int getId() const;
    std::string getDescription() const;

private:
    int id;
    std::string description;
};

#endif // TASK_H