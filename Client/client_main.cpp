#include <iostream>
#include "sync_tcp_client.hpp"
#include "async_tcp_client.hpp"

bool runSyncTcpClient(int argc, char** argv)
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
        std::cout << "Error occured! Error code = " << er.code()
            << ". Message: " << er.what();

        return er.code().value();
    }

    return 0;
}

void handler(unsigned int request_id,
    const std::string& response,
    const boost::system::error_code& ec)
{
    if (ec.value() == 0)
    {
        std::cout << "Request #" << request_id
            << " has completed. Response: "
            << response << std::endl;
    }
    else if (ec == asio::error::operation_aborted)
    {
        std::cout << "Request #" << request_id
            << " has been cancelled by the user."
            << std::endl;
    }
    else
    {
        std::cout << "Request #" << request_id
            << " failed! Error code = " << ec.value()
            << ". Error message = " << ec.message()
            << std::endl;
    }
    return;
}

bool runAsyncTcpClient(int argc, char** argv)
{
    try
    {
        AsyncTcpClient client;
        // Here we emulate the user's behavior.
        // User initiates a request with id 1.
        client.emulateLongComputationOp(10, "127.0.0.1", 3333,
            handler, 1);
            // Then does nothing for 5 seconds.
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // Then initiates another request with id 2.
        client.emulateLongComputationOp(11, "127.0.0.1", 3334,
            handler, 2);
            // Then decides to cancel the request with id 1.
        client.cancelRequest(1);
        // Does nothing for another 6 seconds.
        std::this_thread::sleep_for(std::chrono::seconds(6));
        // Initiates one more request assigning ID3 to it.
        client.emulateLongComputationOp(12, "127.0.0.1", 3335,
            handler, 3);
            // Does nothing for another 15 seconds.
        std::this_thread::sleep_for(std::chrono::seconds(15));
        // Decides to exit the application.
        client.close();
    }
    catch (boost::system::system_error& er)
    {
        std::cout << "Error occured! Error code = " << er.code()
            << ". Message: " << er.what();

        return er.code().value();
    }

    return 0;
}

int main(int argc, char** argv)
{
    //return runSyncTcpClient(argc, argv);
    return runAsyncTcpClient(argc, argv);
}