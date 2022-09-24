#include <iostream>
#include "sync_tcp_client.hpp"

int main(int argc, char** argv)
{
    std::string rawAddress("127.0.0.1");
    u_short port = 3333;

    try
    {
        SyncTcpClient client(rawAddress, port);
        client.connect();

        std::cout << "Enter message to server: ";
        std::string message;
        std::cin >> message;

        client.send(message);

        std::getchar();

        client.close();
    }
    catch (boost::system::system_error& er)
    {
        std::cout << er.what() << " " << er.code()<< std::endl;
    }
}