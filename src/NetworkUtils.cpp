#include "NetworkUtils.h"
#include <WS2tcpip.h>
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

void NetworkUtils::initialize() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void NetworkUtils::cleanup() {
    WSACleanup();
}

void NetworkUtils::sendTask(const std::string& taskData) {
    std::cout << "Send task called" << std::endl;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081); // Ensure this port matches the server's port
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(sock);
        return;
    }

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        return;
    }

    int bytesSent = send(sock, taskData.c_str(), taskData.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Successfully sent " << bytesSent << " bytes to the server." << std::endl;
    }

    closesocket(sock);
}

std::string NetworkUtils::receiveTask() {
    std::cout << "Receive task called" << std::endl;
    SOCKET serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024] = {0};

    // Create socket
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        return "";
    }

    // Set up the sockaddr_in structure for the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081); // Ensure this port matches the client's port
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    // Bind the socket
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        return "";
    }

    // Listen for incoming connections
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        return "";
    }

    // Accept a client connection
    clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        return "";
    }

    std::cout << "Client connected successfully!" << std::endl;

    // Receive data from the client
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::string receivedData(buffer);
        closesocket(clientSock);
        closesocket(serverSock);
        std::cout << "Received " << bytesReceived << " bytes: " << receivedData << std::endl;
        return receivedData;
    } else if (bytesReceived == 0) {
        std::cout << "Connection closed by client." << std::endl;
    } else {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
    }

    closesocket(clientSock);
    closesocket(serverSock);
    return "";
}
