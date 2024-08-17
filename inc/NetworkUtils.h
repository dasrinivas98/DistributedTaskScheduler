#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <string>
#include <iostream>
#include <winsock2.h>

class NetworkUtils {
public:
    static void initialize();
    static void cleanup();
    static void sendTask(const std::string& taskData);
    static std::string receiveTask();
};

#endif // NETWORKUTILS_H
