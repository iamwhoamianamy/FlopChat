#pragma once
#include <boost/asio.hpp>

namespace asio = boost::asio;

class SyncTcpClient
{
private:
    asio::io_service m_ios;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::socket m_socket;

public:
    SyncTcpClient(const std::string& rawIpAddress, u_short port);
    void connect();
    void close();
    void send(const std::string& message);
    std::string recieve();
};

