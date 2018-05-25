#include <boost/bind.hpp>

#include "tcp/session.h"
#include "common/logger.h"
#include "tcp/server.h"

namespace tcp {

Session::Session(const net::SessionAccessor& accessor,
                 boost::asio::io_service& io_service)
    : net::Session(accessor), socket_(io_service) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
  state_ = net::Session::State::RUN;
}

Session::~Session() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void Session::Read() {
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&Session::HandleRead, this,
          boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::Write(BufferPtr buffer) {
    boost::asio::async_write(socket_,
        boost::asio::buffer(*buffer), boost::bind(&Session::HandleWrite, this,
                    boost::asio::placeholders::error, buffer));
}

void Session::Close() {
  socket_.cancel();
  socket_.close();
}

void Session::HandleRead(const boost::system::error_code& error,
                         size_t bytes_transferred) {
  DLOG(INFO) << __PRETTY_FUNCTION__
             << " (" << bytes_transferred << ")";

  if (!error) {
    accessor_.handler.HandleData(shared_from_this(),
        boost::asio::buffer(buffer_.data(), bytes_transferred));
    Read();
  } else {
    LOG(WARNING) << error.message();
    Destroy();
  }
}

void Session::HandleWrite(const boost::system::error_code& error,
                          net::Session::BufferPtr buffer) {
  IGNORE(buffer);
  DLOG(INFO) << __PRETTY_FUNCTION__;

  if (!error) {
    accessor_.handler.HandleWriteComplete(shared_from_this());
  } else {
    LOG(WARNING) << error.message();
    Destroy();
  }
}

void Session::Destroy() {
  state_ = net::Session::State::CLOSED;
  accessor_.handler.HandleClose(shared_from_this());
  accessor_.dispatcher.CloseSession(accessor_);
}

} // namespace server
