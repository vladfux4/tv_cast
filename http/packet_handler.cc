#include "common/logger.h"

#include "http/packet_handler.h"

namespace http {

PacketHandler::PacketHandler()
  : observer_(nullptr),
    current_http_parser_(nullptr) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

PacketHandler::~PacketHandler() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  DeleteHttpParser();
}

net::PacketHandler::Status PacketHandler::Handle(
    net::SessionPtr session,
    const boost::asio::const_buffer& buffer) {
  CreateHttpParser();

  Status status = current_http_parser_->Parse(buffer);

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
    session->Close();
  }

  return status;
}

void PacketHandler::HandleClose(net::SessionPtr session) {
  if (nullptr != observer_) {
    observer_->HandleClose(session);
  }

  DeleteHttpParser();
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

PacketHandlerCreator::PacketHandlerCreator(Packet::Observer& observer)
  : observer_(observer) {
}

PacketHandlerCreator::~PacketHandlerCreator() {
}

net::PacketHandler* PacketHandlerCreator::Create() {
  PacketHandler* handler = new http::PacketHandler();
  handler->RegisterObserver(observer_);

  return handler;
}

void PacketHandlerCreator::Delete(net::PacketHandler* handler) {
  delete handler;
}

}  // namespace http
