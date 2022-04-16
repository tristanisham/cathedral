#include "cathedral.hpp"
using namespace ct; // Cathedrial's namespace
// Starts the pre-configured webserver.
int Server::start()
{
    if (this->sockfd == -1)
    {
        std::cerr << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(this->config.port); // htons is necessary to convert a number to
                                                  // network byte order
    if (bind(this->sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cerr << "Failed to bind to port " << this->config.port << ". errno: " << errno << std::endl
                  << "This is part of the TCP protocol definition. Before being closed, a socket transitions to TIME_WAIT state. This is done to give time to the socket to cleanly shutdown. After some time, the address will be released by the OS." << std::endl;

        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on :" << this->config.port << std::endl;

    if (listen(this->sockfd, 10) < 0)
    {
        std::cerr << "Failed to listen on socket. errno: " << errno << "(Address already in use)" << std::endl
                  << "This is part of the TCP protocol definition. Before being closed, a socket transitions to TIME_WAIT state. This is done to give time to the socket to cleanly shutdown. After some time, the address will be released by the OS." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        auto addrlen = sizeof(sockaddr);
        int connection = accept(this->sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
        if (connection < 0)
        {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        // Read from the connection
        char buffer[100];
        auto bytesRead = read(connection, buffer, 100);
        std::cout << buffer;

        // Send a message to the connection
        std::string response = respond(buffer);
        send(connection, response.c_str(), response.size(), 0);
        close(connection);
    }

    return 0;
}
