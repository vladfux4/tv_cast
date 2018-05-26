#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"
#include "app_ctrl.h"

#include "anet/net/packet_observer.h"

CommunicationController::CommunicationController()
  : io_service_(),
    udp_server_(io_service_, boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string("0.0.0.0"), 8001)),
    tcp_client_(io_service_),
    tcp_server_(io_service_, 49080),
    udp_server_bridge_(nullptr),
    http_client_bridge_(nullptr),
    http_server_bridge_(nullptr),
    http_client_observer_creator_(nullptr),
    http_server_observer_creator_(nullptr) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

CommunicationController::~CommunicationController() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}
anet::tcp::SessionPtr CommunicationController::CreateHttpClientSession(
    const boost::asio::ip::tcp::endpoint& target) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  return tcp_client_.CreateSession(target);
}

void CommunicationController::RegisterAppController(
    ApplicationController& ctrl) {
  LOG(INFO) << __PRETTY_FUNCTION__;

  udp_server_bridge_.reset(
      new UdpServerBridge(ctrl));
  CHECK_NOTNULL(udp_server_bridge_);

  udp_server_.RegisterObserver(*udp_server_bridge_);

  http_client_bridge_.reset(
      new HttpClientBridge(ctrl));
  CHECK_NOTNULL(http_client_bridge_);

  http_client_observer_creator_.reset(
      new anet::http::SessionObserverCreator(*http_client_bridge_));
  tcp_client_.RegisterCreator(*http_client_observer_creator_);

  http_server_bridge_.reset(
      new HttpServerBridge(ctrl));
  CHECK_NOTNULL(http_server_bridge_);

  http_server_observer_creator_.reset(
      new anet::http::SessionObserverCreator(*http_server_bridge_));
  tcp_server_.RegisterCreator(*http_server_observer_creator_);
}

void CommunicationController::Run() {
  LOG(INFO) << __PRETTY_FUNCTION__;

  tcp_server_.Start();

  udp_server_.JoinMulticastGroup(
      boost::asio::ip::address::from_string("224.0.0.7"));
  udp_server_.Start();

  io_service_.run();
}

anet::net::PacketObserver::Status UdpServerBridge::HandleData(
    const boost::asio::const_buffer& buffer) {
  ctrl_.HandleUdpData(buffer);

  return anet::net::PacketObserver::Status::OK;
}

void UdpServerBridge::HandleWriteComplete() {
  ctrl_.NotifyUdpDataSend();
}

void HttpServerBridge::HandlePacket(
    anet::net::SessionPtr session,
    const anet::http::Packet& packet) {
  ctrl_.HandleLoopPacket(session, packet);
}

void HttpServerBridge::HandleWriteComplete(anet::net::SessionPtr session) {
  ctrl_.NotifyLoopDataSend(session);
}

void HttpServerBridge::HandleClose(anet::net::SessionPtr session) {
  ctrl_.NotifyLoopSessionClose(session);
}

void HttpClientBridge::HandlePacket(
    anet::net::SessionPtr session,
    const anet::http::Packet& packet) {
  ctrl_.HandleServicePacket(session, packet);
}

void HttpClientBridge::HandleWriteComplete(anet::net::SessionPtr session) {
  ctrl_.NotifyServiceDataSend(session);
}

void HttpClientBridge::HandleClose(anet::net::SessionPtr session) {
  ctrl_.NotifyServiceSessionClose(session);
}
