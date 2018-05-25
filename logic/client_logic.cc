#include "logic/client_logic.h"
#include "common/logger.h"

#include "com_ctrl.h"

namespace logic {

ClientLogic::ClientLogic(CommunicationController& com_ctrl)
    : com_ctrl_(com_ctrl) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;
}

void ClientLogic::SendGetRequest() {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;

  tcp::SessionPtr session = com_ctrl_.CreateHttpClientSession(
      boost::asio::ip::tcp::endpoint(
          boost::asio::ip::address::from_string("93.188.2.51"), 80));
  if (nullptr != session) {
    http::Packet packet;
    packet.Init(1,1, http::Packet::Status::INIT, http::Packet::Method::GET,
                http::Packet::Type::REQUEST);
    packet.SetUrl("http://93.188.2.51/");
    packet.AddHeaderField("Connection", "keep-alive");
    packet.AddHeaderField("Host", "93.188.2.51");

    net::Session::BufferPtr new_buffer = packet.Serialize();
    if (nullptr != new_buffer) {
      LOG(LogLevel::INFO) << "REQUEST NEW HTTP PACKET";
      LOG(LogLevel::INFO)
          << std::string(reinterpret_cast<char*>(&new_buffer->at(0)),
                         new_buffer->size());

      session->Write(new_buffer);
      session->Read();
    }
  } else {
    LOG(LogLevel::INFO) << "SESSION NOT CREATED";
  }
}

void ClientLogic::HandlePacket(net::SessionPtr session,
                         const http::Packet& packet) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;

  net::Session::BufferPtr buffer = packet.Serialize();

  if (nullptr != buffer) {
    LOG(LogLevel::INFO) << "HTTP PACKET";
    LOG(LogLevel::INFO)
        << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                       buffer->size());
  }
}

void ClientLogic::HandleClose(net::SessionPtr session) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;
}

}  // namespace logic
