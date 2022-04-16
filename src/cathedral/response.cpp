#include "cathedral.hpp"
using namespace ct; // Cathedrial's namespace

std::string Server::respond(const char buffer[100])
{
    std::string first_row;
    std::string method, route, protocol;
    std::string buf{buffer};
    int count = 0;
    for (size_t i = 0; i < buf.find_first_of("\r\n"); i++)
    {
        if (buf[i] == ' ') {
            count++;
            continue;
        }
        first_row.append(std::string{buf[i]});
    }

    auto first_row_words = split(first_row, ' ');
    if (first_row_words.size() == 3) {
        method = first_row_words[0];
        route = first_row_words[1];
        protocol = first_row_words[2];

    }

    Context ctx;
    ctx.from(buffer);
    // this->routes[ctx.getMethod()][ctx.getRoute()]

    // return "HTTP/1.1 200 OK\r\nServer: Cathedral 0.0.1\r\n\r\n<p>Hello, World!</p>";
    return this->routes[ctx.getMethod()][ctx.getRoute()].response();
}

void Server::get(const std::string &path, std::function<void(Context)> handler)
{
    Context ctx{handler};
    this->routes["GET"][path] = ctx;
}
void Server::post(const std::string &path, std::function<void(Context)> handler)
{
    Context ctx{handler};
    this->routes["POST"][path] = ctx;
}
void Server::put(const std::string &path, std::function<void(Context)> handler)
{
    Context ctx{handler};
    this->routes["PUT"][path] = ctx;
}
void Server::method(const std::string &method, std::string &path, std::function<void(Context)> handler)
{
    Context ctx{handler};
    this->routes[method][path] = ctx;
}
