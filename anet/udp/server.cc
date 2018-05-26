#include <boost/bind.hpp>

#include "anet/udp/server.h"
#include "common/logger.h"

namespace anet {
namespace udp {

Server::Server(boost::asio::io_service& io_service,
               const boost::asio::ip::udp::endpoint& listen_endpoint)
    : io_service_(io_service),
      observer_(nullptr),
      socket_(io_service),
      sender_endpoint_(),
      buffer_() {
  DLOG(INFO) << __PRETTY_FUNCTION__ << "(" << listen_endpoint.port() << ")";

  socket_.open(listen_endpoint.protocol());
  socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  socket_.bind(listen_endpoint);
}

Server::~Server() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void Server::RegisterObserver(net::PacketObserver& observer) {
  observer_ = &observer;
}

void Server::JoinMulticastGroup(
    const boost::asio::ip::address& multicast_address) {
  socket_.set_option(
        boost::asio::ip::multicast::join_group(multicast_address));
}

void Server::Connect(const boost::asio::ip::udp::endpoint& target) {
  socket_.connect(target);
}

void Server::Start() {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  Receive();
}

void Server::Receive() {
  socket_.async_receive_from(boost::asio::buffer(buffer_), sender_endpoint_,
      boost::bind(&Server::HandleReceive, this,
          boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Server::Send(BufferPtr buffer) {
    socket_.async_send(
        boost::asio::buffer(*buffer), boost::bind(&Server::HandleSend, this,
                    boost::asio::placeholders::error, buffer));
}

void Server::HandleReceive(const boost::system::error_code& error,
                         size_t bytes_transferred) {
  DLOG(INFO) << __PRETTY_FUNCTION__
             << " (" << bytes_transferred << ")";

  if (!error) {
    CHECK_NOTNULL(observer_);
    observer_->HandleData(
        boost::asio::buffer(buffer_.data(), bytes_transferred));
    Receive();
  } else {
    LOG(WARNING) << error.message();
  }
}

void Server::HandleSend(const boost::system::error_code& error,
                          net::Session::BufferPtr buffer) {
  IGNORE(buffer);
  DLOG(INFO) << __PRETTY_FUNCTION__;

  if (!error) {
    CHECK_NOTNULL(observer_);
    observer_->HandleWriteComplete();
  } else {
    LOG(WARNING) << error.message();
  }
}


} // namespace udp
} // namespace anet
