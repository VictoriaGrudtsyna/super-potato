#include "server.hpp"
#include "session.hpp"

Server::Server(boost::asio::io_context& io_context, short port)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    accept();
}

void Server::accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            } else {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            }
            accept();
        });
}