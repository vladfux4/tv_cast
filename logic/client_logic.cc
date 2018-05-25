#include "logic/client_logic.h"
#include "common/logger.h"

#include "com_ctrl.h"

namespace logic {

ClientLogic::ClientLogic(CommunicationController& com_ctrl)
    : com_ctrl_(com_ctrl) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void ClientLogic::SendGetRequest() {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  anet::tcp::SessionPtr session = com_ctrl_.CreateHttpClientSession(
      boost::asio::ip::tcp::endpoint(
          boost::asio::ip::address::from_string("93.188.2.51"), 80));
  if (nullptr != session) {
    anet::http::Packet packet;
    packet.Init(1,1, anet::http::Packet::Status::INIT,
        anet::http::Packet::Method::GET, anet::http::Packet::Type::REQUEST);
    packet.SetUrl("http://93.188.2.51/");
    packet.AddHeaderField("Connection", "keep-alive");
    packet.AddHeaderField("Host", "93.188.2.51");

    anet::net::Session::BufferPtr new_buffer = packet.Serialize();
    if (nullptr != new_buffer) {
      DLOG(INFO) << "REQUEST NEW HTTP PACKET";
      VLOG(2)
          << std::string(reinterpret_cast<char*>(&new_buffer->at(0)),
                         new_buffer->size());

      session->Write(new_buffer);
      session->Read();
    }
  } else {
    DLOG(WARNING) << "SESSION NOT CREATED";
  }
}

void ClientLogic::HandlePacket(anet::net::SessionPtr session,
                         const anet::http::Packet& packet) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  anet::net::Session::BufferPtr buffer = packet.Serialize();

  if (nullptr != buffer) {
    DLOG(INFO) << "HTTP PACKET";
    VLOG(2)
        << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                       buffer->size());
  }
}

void ClientLogic::HandleWriteComplete(anet::net::SessionPtr session) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void ClientLogic::HandleClose(anet::net::SessionPtr session) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

}  // namespace logic
