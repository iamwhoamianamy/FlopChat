#pragma once
#include <boost/asio.hpp>
#include <boost/predef.h>
#include <boost/core/noncopyable.hpp>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <map>

#ifdef BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501

    #if _WIN32_WINNT <= 0x0502 // Windows Server 2003 or earlier.
        #define BOOST_ASIO_DISABLE_IOCP
        #define BOOST_ASIO_ENABLE_CANCELIO
    #endif

#endif

namespace asio = boost::asio;

typedef void(*Callback) (
    u_int request_id,
    const std::string& response,
    const boost::system::error_code& ec);

struct Session
{
    Session(
        asio::io_service& ios,
        const std::string& rawIpAddress,
        u_short port,
        const std::string& request,
        u_int id,
        Callback callback) :
            m_sock(ios),
            m_ep(asio::ip::address::from_string(rawIpAddress),
                port),
            m_request(request),
            m_id(id),
            m_callback(callback),
            m_was_cancelled(false)
    {
    }

    asio::ip::tcp::socket m_sock;
    asio::ip::tcp::endpoint m_ep;
    std::string m_request;
    asio::streambuf m_response_buf;
    std::string m_response;
    boost::system::error_code m_ec;
    u_short m_id;
    Callback m_callback;
    bool m_was_cancelled;
    std::mutex m_cancel_guard;
};

class AsyncTcpClient : public boost::noncopyable
{
private:
    asio::io_service m_ios;
    std::map<int, std::shared_ptr<Session>> m_active_sessions;
    std::mutex m_active_sessions_guard;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;

public:
    AsyncTcpClient();

    void emulateLongComputationOp(
        unsigned int duration_sec,
        const std::string& raw_ip_address,
        unsigned short port_num,
        Callback callback,
        unsigned int request_id);

    void cancelRequest(unsigned int request_id);
    void close();

private:
    void onRequestComplete(std::shared_ptr<Session> session);
};

