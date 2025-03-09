#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace http = boost::beast::http;
namespace tcp = boost::asio::ip::tcp;
using tcp = boost::asio::ip::tcp;

class Session;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void accept();

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

#endif // SERVER_HPP