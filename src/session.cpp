#include "session.hpp"
#include "handlers.hpp"
#include "handler_request.hpp"
#include <iostream>

Session::Session(tcp::socket socket)
    : socket_(std::move(socket)) {} 

void Session::start() {
    read_request();
}

void Session::read_request() {
    auto self = shared_from_this();
    http::async_read(
        socket_,
        buffer_,
        request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) mutable {
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                self->process_request();
            } else {
                std::cerr << "Error reading request: " << ec.message() << std::endl;
            }
        });
}

void Session::process_request() {
    try {
        handle_request(request_, response_);
        write_response();
    } catch (const std::exception& e) {
        std::cerr << "Error processing request: " << e.what() << std::endl;
        response_.result(http::status::internal_server_error);
        response_.set(http::field::content_type, "application/json");
        response_.body() = R"({"error": "Internal server error"})";
        write_response();
    }
}

void Session::write_response() {
    auto self = shared_from_this();
    http::async_write(
        socket_,
        response_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (ec) {
                std::cerr << "Error writing response: " << ec.message() << std::endl;
            }
            self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        });
}