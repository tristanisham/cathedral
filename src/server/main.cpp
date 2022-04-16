#include "../cathedral/cathedral.hpp"
int main()
{
    ct::Server server{};

    server.get("/", [](ct::Context c){
        c.send_string(200, "Hello, world!");
    });

    return server.start();
}