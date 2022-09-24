#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;

std::string read(asio::ip::tcp::socket& socket)
{
    std::string result;
    asio::streambuf buf;

    try
    {
        asio::read(socket, buf);
    }
    catch (boost::system::system_error&)
    {
        std::istream input(&buf);
        result = std::string(std::istreambuf_iterator<char>(input), {});
    }

    return result;
}

int main(int argc, char** argv)
{
    asio::io_service ios;
    asio::ip::tcp::acceptor acceptor(ios);

    // Acceptor openning
    try
    {
        acceptor.open(asio::ip::tcp::v4());
        std::cout << "Successfull opening of an acceptor!" << std::endl;
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }

    // Binding an endpoint to an acceptor
    try
    {
        u_short port = std::atoi(argv[1]);
        asio::ip::tcp::endpoint endpoint(asio::ip::address_v4::any(), port);

        acceptor.bind(endpoint);
        std::cout << "Successfull binding of an endpoint!" << std::endl;
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }

    // Listening and ascepting clients
    try
    {
        acceptor.listen(10);
        std::cout << "Successfull started listening!" << std::endl;

        asio::ip::tcp::socket client(ios);
        acceptor.accept(client);
        std::cout << "Accepted a client from " << client.local_endpoint().address() << std::endl;

        std::string message = read(client);

        std::cout << "Client's message: " << message << std::endl;
        std::getchar();
    }
    catch (boost::system::system_error& error)
    {
        std::cout << error.code() << " " << error.what() << std::endl;
        std::getchar();
        return 0;
    }
}