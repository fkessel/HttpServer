#include <vector>
#include <memory>

#include "include/Server.hpp"
#include "include/Connection.hpp"
#include "include/HttpMessage/Request.hpp"
#include "include/HttpMessage/Response.hpp"

using namespace http;

int main(int argc, char *argv[])
{
    try {
    Server srv(65000, AF_INET);
    std::vector< std::shared_ptr<Connection> > connections;

    while (true)
    {
        connections.push_back(srv.Accept([](Connection& con){
            std::string buffer;
            con.readMessage(buffer);
            std::cout << buffer << std::endl;
            
            Request request(buffer);

            Response response(request);
            con.writeMessage(response.generateResponse());
            con.closeSocket();
        }));
    }
    } catch (const ServerException& se) { se.what(); }
    // Never reached. Deconstructor is closing Server
    return 0;
}