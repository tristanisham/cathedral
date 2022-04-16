#pragma once
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in
#include <cstdlib>       // For exit() and EXIT_FAILURE
#include <iostream>      // For cout
#include <unistd.h>      // For read
#include <csignal>       // For SIGINT
#include <unordered_map> // For routes
#include <functional>    // For functions
#include <sstream>       // For parsing http request
#include <vector>        // Becasue why the fuck not?

namespace ct
{

    std::vector<std::string> split(std::string source, const char &deliminator);
    // Response represents a response body for Context
    struct Response
    {
        std::string header;
        std::string body;
        std::string status;
        std::string status_msg;
        std::string nl = "\r\n";
    };

    class Context
    {
        std::string method;
        std::string route;
        std::unordered_map<std::string, std::string> params;
        std::string protocol;
        std::function<void(Context)> func;
        std::unordered_map<std::string, std::string> headers;
        std::vector<std::vector<std::string>> body;

        std::string static status_code_message(const int &code);

    public:
        Context(){};
        Context(std::function<void(Context)> handler)
        {
            this->func = handler;
        };
        // from creates a fills a context from a buffer.
        void from(std::string request);
        void header(std::string key, std::string value);

        // response() returns the complete HTTP response that can be sent to the client.
        std::string response();
        std::string getMethod()
        {
            return this->method;
        }
        std::string getRoute()
        {
            return this->route;
        }
        Response rsp;

        // USER FUNCTIONS
        void send_string(int code, std::string body);
    };

    struct ServerConfig
    {
        uint port = 8080;
    };

    class Server
    {
        ServerConfig config;

    private:
        int sockfd;
        // Method | Route | Func
        std::unordered_map<std::string, std::unordered_map<std::string, Context>> routes;
        std::string respond(const char buffer[100]);

    public:
        int start();
        Server()
        {
            this->config = ServerConfig{};
            this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

        } // Default constructor
        Server(uint port)
        {
            this->config = ServerConfig{
                port};
            this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        }

        ~Server()
        {
            close(this->sockfd);
        }

        // get adds a http response to a specific GET method on provided path.
        void get(const std::string &path, std::function<void(Context)> handler);
        void post(const std::string &path, std::function<void(Context)> handler);
        void put(const std::string &path, std::function<void(Context)> handler);
        void method(const std::string &method, std::string &path, std::function<void(Context)> handler);
        // void options(const std::string &path, std::function<int(Context)> handler);
        // void head(const std::string &path, std::function<int(Context)> handler);
        // void trace(const std::string &path, std::function<int(Context)> handler);
        // void connect(const std::string &path, std::function<int(Context)> handler);
    };
}