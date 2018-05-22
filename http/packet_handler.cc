#include "common/logger.h"

#include "http/packet_handler.h"

namespace http {

PacketHandler::PacketHandler()
  : observer_(nullptr),
    current_http_parser_(nullptr) {
}

PacketHandler::~PacketHandler() {
  DeleteHttpParser();
}

net::PacketHandler::Status PacketHandler::Handle(
    net::Session& session,
    const boost::asio::const_buffer& buffer) {
  Status status = Status::ERROR;

  do {
    if (0 == buffer.size()) {
      DeleteHttpParser();
      break;
    }

    CreateHttpParser();

    status = current_http_parser_->Parse(buffer);

    if (Status::PART_RECEIVED == status) {
      LOG(LogLevel::DEBUG) << "Waiting for rest of package";
    } else if (Status::OK == status) {
      http::Parser::PacketPtr packet = current_http_parser_->GetPacket();
      if (nullptr != observer_) {
        observer_->HandlePacket(session, *packet);
      }

      DeleteHttpParser();
    } else if (Status::ERROR == status) {
      DeleteHttpParser();
      session.Close();
    }
  } while (false);

  return status;
}

void PacketHandler::RegisterObserver(Packet::Observer& observer) {
  observer_ = &observer;
}

void PacketHandler::CreateHttpParser() {
  if (nullptr == current_http_parser_) {
    current_http_parser_ = new http::Parser();
  }
}

void PacketHandler::DeleteHttpParser() {
  if (nullptr != current_http_parser_) {
    delete current_http_parser_;
    current_http_parser_ = nullptr;
  }
}

}  // namespace http
