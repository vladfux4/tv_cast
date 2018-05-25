#include "common/logger.h"

#include "http/session_handler.h"

namespace http {

SessionHandler::SessionHandler()
  : observer_(nullptr),
    current_http_parser_(nullptr) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

SessionHandler::~SessionHandler() {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  DeleteHttpParser();
}

net::SessionHandler::Status SessionHandler::HandleData(
    net::SessionPtr session,
    const boost::asio::const_buffer& buffer) {
  CreateHttpParser();

  Status status = current_http_parser_->Parse(buffer);

  if (Status::PART_RECEIVED == status) {
    DLOG(INFO) << "Waiting for rest of package";
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

void SessionHandler::HandleWriteComplete(net::SessionPtr session) {
  if (nullptr != observer_) {
    observer_->HandleWriteComplete(session);
  }
}

void SessionHandler::HandleClose(net::SessionPtr session) {
  if (nullptr != observer_) {
    observer_->HandleClose(session);
  }

  DeleteHttpParser();
}

void SessionHandler::RegisterObserver(Packet::Observer& observer) {
  observer_ = &observer;
}

void SessionHandler::CreateHttpParser() {
  if (nullptr == current_http_parser_) {
    current_http_parser_ = new http::Parser();
  }
}

void SessionHandler::DeleteHttpParser() {
  if (nullptr != current_http_parser_) {
    delete current_http_parser_;
    current_http_parser_ = nullptr;
  }
}

SessionHandlerCreator::SessionHandlerCreator(Packet::Observer& observer)
  : observer_(observer) {
}

SessionHandlerCreator::~SessionHandlerCreator() {
}

net::SessionHandler* SessionHandlerCreator::Create() {
  SessionHandler* handler = new http::SessionHandler();
  handler->RegisterObserver(observer_);

  return handler;
}

void SessionHandlerCreator::Delete(net::SessionHandler* handler) {
  delete handler;
}

}  // namespace http
