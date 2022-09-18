#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;

int main(int argc, char** argv)
{
    asio::io_service ios;
    asio::ip::tcp::socket socket(ios);

    // Socket opening
    try
    {
        socket.open(asio::ip::tcp::v4());
        std::cout << "Successfull opening of a socket!" << std::endl;
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }

    std::string rawIp = "127.0.0.1";
    u_short port = 3333;
    asio::ip::address_v4 address;

    // Address parsing
    try
    {
        address = asio::ip::address_v4::from_string(rawIp);
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }

    asio::ip::tcp::endpoint endpoint(address, port);

    // Connecting a socket to an endpoint
    try
    {
        socket.connect(endpoint);
        std::cout << "Successfull connection to a server!" << std::endl;

        asio::write(socket, asio::buffer("Hello, server!\nI am a client!!!!"));
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }
}