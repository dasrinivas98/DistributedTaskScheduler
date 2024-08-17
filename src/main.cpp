#include "NetworkUtils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

// Function to run the server (receive task)
void runServer() {
    try {
        NetworkUtils::initialize();
        std::string receivedTask = NetworkUtils::receiveTask();
        std::cout << "Received task data: " << receivedTask << std::endl;
        NetworkUtils::cleanup();
    } catch (const std::exception& e) {
        std::cerr << "Server Error: " << e.what() << std::endl;
    }
}

// Function to run the client (send task)
void runClient() {
    // Delay the client slightly to ensure the server is listening
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    try {
        NetworkUtils::initialize();
        NetworkUtils::sendTask("Task 1 data");
        NetworkUtils::cleanup();
    } catch (const std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
    }
}

int main() {
    std::ofstream outFile("execution_log.txt");
    std::ofstream errFile("error_log.txt");
    
    // Redirect std::cout and std::cerr to log files
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::streambuf* cerrBuf = std::cerr.rdbuf();

    std::cout.rdbuf(outFile.rdbuf());
    std::cerr.rdbuf(errFile.rdbuf());
    // Create threads for server and client
    std::thread serverThread(runServer);
    std::thread clientThread(runClient);

    // Wait for both threads to finish
    serverThread.join();
    clientThread.join();

    return 0;
}
