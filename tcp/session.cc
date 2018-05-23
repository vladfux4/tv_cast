#include <boost/bind.hpp>

#include "tcp/session.h"
#include "common/logger.h"
#include "tcp/server.h"

namespace tcp {

Session::Session(const net::SessionAccessor& accessor,
                 boost::asio::io_service& io_service)
    : net::Session(accessor), socket_(io_service) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
  state_ = net::Session::State::RUN;
}

Session::~Session() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

void Session::Read() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&Session::HandleRead, this,
          boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::Write(const boost::asio::const_buffer& buffer) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

    boost::asio::async_write(socket_,
        buffer, boost::bind(&Session::HandleWrite, this,
                    boost::asio::placeholders::error));
}

void Session::Close() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  socket_.cancel();
  socket_.close();
}

void Session::HandleRead(const boost::system::error_code& error,
                         size_t bytes_transferred) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  if (!error) {
    accessor_.dispatcher.Handle(shared_from_this(),
        boost::asio::buffer(buffer_.data(), bytes_transferred));
    Read();
  } else {
    LOG(LogLevel::DEBUG) << error.message();

    Destroy();
  }
}

void Session::HandleWrite(const boost::system::error_code& error) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  if (error) {
    LOG(LogLevel::DEBUG) << error.message();
    Destroy();
  }
}

void Session::Destroy() {
  state_ = net::Session::State::CLOSED;
  accessor_.dispatcher.HandleClose(shared_from_this());
  accessor_.dispatcher.CloseSession(accessor_);
}

} // namespace server
