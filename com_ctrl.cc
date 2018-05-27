#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"
#include "app_ctrl.h"

#include "anet/net/packet_observer.h"

namespace {

const uint16_t SERVICE_PORT = 8001;
const uint16_t LOOP_PORT = 49080;
const char* SERVICE_ADDRESS = "224.0.0.7";

}

CommunicationController::CommunicationController(
    boost::asio::io_service& io_service)
  : io_service_(io_service),
    udp_server_(io_service_, boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string("0.0.0.0"), SERVICE_PORT)),
    tcp_client_(io_service_),
    tcp_server_(io_service_, LOOP_PORT),
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

void CommunicationController::Start() {
  LOG(INFO) << __PRETTY_FUNCTION__;

  tcp_server_.Start();

  udp_server_.JoinMulticastGroup(
      boost::asio::ip::address::from_string(SERVICE_ADDRESS));

  udp_server_.Start();
}

anet::tcp::SessionPtr CommunicationController::CreateHttpClientSession(
    const boost::asio::ip::address& address) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  return tcp_client_.CreateSession(
      boost::asio::ip::tcp::endpoint(address, SERVICE_PORT));
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
