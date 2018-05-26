#include "logic/server_logic.h"
#include "common/logger.h"

namespace logic {

ServerLogic::ServerLogic() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void ServerLogic::HandlePacket(anet::net::SessionPtr session,
                         const anet::http::Packet& packet) {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  anet::net::Session::BufferPtr buffer = packet.Serialize();

  if (nullptr != buffer) {
    DLOG(INFO) << "HTTP PACKET";
    VLOG(2)
        << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                       buffer->size());
  }
/*
  anet::http::Packet new_packet;
  new_packet.Init(1,1, anet::http::Packet::Status::OK,
      anet::http::Packet::Method::INIT, anet::http::Packet::Type::RESPONSE);
  new_packet.AddHeaderField("Content-Type", "text/html; charset=utf-8");
  anet::http::Packet::BufferPtr body(
      new anet::http::Packet::Buffer(buffer->begin(), buffer->end()));
  new_packet.AddHeaderField("Content-Length", std::to_string(body->size()));
  new_packet.AssignBody(boost::move(body));

  anet::net::Session::BufferPtr new_buffer = new_packet.Serialize();
  if (nullptr != new_buffer) {
    DLOG(INFO) << "NEW HTTP PACKET";
    VLOG(2)
        << std::string(reinterpret_cast<char*>(&new_buffer->at(0)),
                       new_buffer->size());

    session->Write(new_buffer);
  }
*/
}

void ServerLogic::HandleWriteComplete(anet::net::SessionPtr session) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
//  session->Close();
}

void ServerLogic::HandleClose(anet::net::SessionPtr session) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

}  // namespace logic
