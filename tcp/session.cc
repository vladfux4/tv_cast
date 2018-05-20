#include <boost/bind.hpp>

#include "tcp/session.h"
#include "common/logger.h"
#include "tcp/server.h"

namespace tcp {

Session::Session(const ServerAccessor& accessor,
                 boost::asio::io_service& io_service)
    : server_accessor_(accessor), socket_(io_service) {
  LOG(LogLevel::DEBUG) << "New session";
}

Session::~Session() {
  LOG(LogLevel::DEBUG) << "Destroy session";
}

void Session::Start() {
  LOG(LogLevel::DEBUG) << "Start session";

  Read();
}

void Session::Close() {
  LOG(LogLevel::DEBUG) << "Close session";

  socket_.cancel();
  socket_.close();
}

void Session::HandleRead(const boost::system::error_code& error,
                         size_t bytes_transferred) {
  LOG(LogLevel::DEBUG) << "HandleRead";

  if (!error) {
    server_accessor_.server.Handle(*this,
        boost::asio::buffer(buffer_.data(), bytes_transferred));
    Read();
  } else {
    LOG(LogLevel::DEBUG) << error.message();

    if (boost::asio::error::eof == error) {
      server_accessor_.server.Handle(*this,
          boost::asio::buffer(buffer_.data(), 0));
    }

    Destroy();
  }
}

void Session::HandleWrite(const boost::system::error_code& error) {
  LOG(LogLevel::DEBUG) << "HandleWrite";

  if (error) {
    LOG(LogLevel::DEBUG) << error.message();
    Destroy();
  }
}

void Session::Write(const boost::asio::const_buffer& buffer) {
    boost::asio::async_write(socket_,
        buffer, boost::bind(&Session::HandleWrite, this,
                    boost::asio::placeholders::error));
}

void Session::Read() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&Session::HandleRead, this,
          boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::Destroy() {
  server_accessor_.server.CloseSession(server_accessor_);
}

} // namespace server
