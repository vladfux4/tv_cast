#include "app_ctrl.h"
#include "common/logger.h"

ApplicationController::ApplicationController() {
}

ApplicationController::~ApplicationController() {
}

void ApplicationController::HandleUdpData(
    const boost::asio::const_buffer& buffer) {
  VLOG(2) << "UDP Data: "
      << std::string(reinterpret_cast<const char*>(buffer.data()),
                     buffer.size());
}

void ApplicationController::NotifyUdpDataSend() {
}

void ApplicationController::HandleServicePacket(
    anet::net::SessionPtr session, const anet::http::Packet& packet) {
  anet::net::Session::BufferPtr data = packet.Serialize();
  VLOG(2) << "Servcie Data: "
      << std::string(reinterpret_cast<const char*>(data->at(0)),
                     data->size());
}

void ApplicationController::NotifyServiceDataSend(
    anet::net::SessionPtr session) {
}

void ApplicationController::NotifyServiceSessionClose(
    anet::net::SessionPtr session) {
}

void ApplicationController::HandleLoopPacket(
    anet::net::SessionPtr session, const anet::http::Packet& packet) {
  anet::net::Session::BufferPtr data = packet.Serialize();
  VLOG(2) << "Servcie Data: "
      << std::string(reinterpret_cast<const char*>(&data->at(0)),
                     data->size());
}

void ApplicationController::NotifyLoopDataSend(
    anet::net::SessionPtr session) {
}

void ApplicationController::NotifyLoopSessionClose(
    anet::net::SessionPtr session) {
}
