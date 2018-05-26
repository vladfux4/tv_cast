#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"

#include "anet/net/packet_observer.h"

class UdpTest : public anet::net::PacketObserver {
 public:
  virtual Status HandleData(const boost::asio::const_buffer& buffer) {
    DLOG(INFO) << __PRETTY_FUNCTION__ << "DATA: "
        << std::string(reinterpret_cast<const char*>(buffer.data()),
                       buffer.size());
    return Status::OK;
  }

  virtual void HandleWriteComplete() {
    DLOG(INFO) << __PRETTY_FUNCTION__;
  }
};

CommunicationController::CommunicationController()
  : io_service_(),
    tcp_server_(io_service_, 49080),
    tcp_client_(io_service_),
    http_handler_creator_(nullptr),
    http_client_handler_creator_(nullptr),
    udp_server_(io_service_, boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string("0.0.0.0"), 8001)) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  static UdpTest udp_test;
  udp_server_.RegisterObserver(udp_test);
}

CommunicationController::~CommunicationController() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void CommunicationController::RegisterHttpServerObserver(
    anet::http::Packet::Observer& observer) {
  LOG(INFO) << __PRETTY_FUNCTION__;

  http_handler_creator_.reset(
      new anet::http::SessionObserverCreator(observer));
  tcp_server_.RegisterCreator(*http_handler_creator_);
}

void CommunicationController::RegisterHttpClientObserver(
    anet::http::Packet::Observer& observer) {
  LOG(INFO) << __PRETTY_FUNCTION__;

  http_client_handler_creator_.reset(
      new anet::http::SessionObserverCreator(observer));
  tcp_client_.RegisterCreator(*http_client_handler_creator_);
}

anet::tcp::SessionPtr CommunicationController::CreateHttpClientSession(
    const boost::asio::ip::tcp::endpoint& target) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  return tcp_client_.CreateSession(target);
}

void CommunicationController::Run() {
  LOG(INFO) << __PRETTY_FUNCTION__;

  tcp_server_.Start();
  udp_server_.JoinMulticastGroup(
      boost::asio::ip::address::from_string("224.0.0.7"));

  udp_server_.Start();

  io_service_.run();
}
