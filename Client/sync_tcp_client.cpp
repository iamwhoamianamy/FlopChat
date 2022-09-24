#include "sync_tcp_client.hpp"

SyncTcpClient::SyncTcpClient(const std::string& rawIpAddress, u_short port) :
    m_endpoint(asio::ip::address::from_string(rawIpAddress), port),
    m_socket(m_ios)
{
    m_socket.open(m_endpoint.protocol());
}

void SyncTcpClient::connect()
{
    m_socket.connect(m_endpoint);
}

void SyncTcpClient::close()
{
    m_socket.shutdown(asio::socket_base::shutdown_both);
    m_socket.close();
}

void SyncTcpClient::send(const std::string& message)
{
    asio::write(m_socket, asio::buffer(message));
}

std::string SyncTcpClient::recieve()
{
    std::string result;
    asio::streambuf buf;

    try
    {
        asio::read(m_socket, buf);
    }
    catch (boost::system::system_error&)
    {
        std::istream input(&buf);
        result = std::string(std::istreambuf_iterator<char>(input), {});
    }

    return result;
}
