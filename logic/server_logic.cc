#include "logic/server_logic.h"
#include "common/logger.h"

namespace logic {

ServerLogic::ServerLogic() {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;
}

void ServerLogic::HandlePacket(net::SessionPtr session,
                         const http::Packet& packet) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;

  net::Session::BufferPtr buffer = packet.Serialize();

  if (nullptr != buffer) {
    LOG(LogLevel::INFO) << "HTTP PACKET";
    LOG(LogLevel::INFO)
        << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                       buffer->size());
  }

  http::Packet new_packet;
  new_packet.Init(1,1, http::Packet::Status::OK, http::Packet::Method::INIT,
              http::Packet::Type::RESPONSE);
  new_packet.AddHeaderField("Content-Type", "text/html; charset=utf-8");
  http::Packet::BufferPtr body(
      new http::Packet::Buffer(buffer->begin(), buffer->end()));
  new_packet.AddHeaderField("Content-Length", std::to_string(body->size()));
  new_packet.AssignBody(boost::move(body));

  net::Session::BufferPtr new_buffer = new_packet.Serialize();
  if (nullptr != new_buffer) {
    LOG(LogLevel::INFO) << "NEW HTTP PACKET";
    LOG(LogLevel::INFO)
        << std::string(reinterpret_cast<char*>(&new_buffer->at(0)),
                       new_buffer->size());

    session->Write(new_buffer);
  }
}

void ServerLogic::HandleWriteComplete(net::SessionPtr session) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;
  session->Close();
}

void ServerLogic::HandleClose(net::SessionPtr session) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;
}

}  // namespace logic
