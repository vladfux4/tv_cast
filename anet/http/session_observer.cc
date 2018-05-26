#include "common/logger.h"

#include "anet/http/session_observer.h"

namespace anet {
namespace http {

SessionObserver::SessionObserver()
  : observer_(nullptr),
    current_http_parser_(nullptr) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

SessionObserver::~SessionObserver() {
  DLOG(INFO) << __PRETTY_FUNCTION__;

  DeleteHttpParser();
}

net::PacketObserver::Status SessionObserver::HandleData(
    net::SessionPtr session,
    const boost::asio::const_buffer& buffer) {
  CreateHttpParser();

  net::PacketObserver::Status status = current_http_parser_->Parse(buffer);

  if (net::PacketObserver::Status::PART_RECEIVED == status) {
    DLOG(INFO) << "Waiting for rest of package";
  } else if (net::PacketObserver::Status::OK == status) {
    http::Parser::PacketPtr packet = current_http_parser_->GetPacket();
    if (nullptr != observer_) {
      observer_->HandlePacket(session, *packet);
    }

    DeleteHttpParser();
  } else if (net::PacketObserver::Status::ERROR == status) {
    DeleteHttpParser();
    session->Close();
  }

  return status;
}

void SessionObserver::HandleWriteComplete(net::SessionPtr session) {
  if (nullptr != observer_) {
    observer_->HandleWriteComplete(session);
  }
}

void SessionObserver::HandleClose(net::SessionPtr session) {
  if (nullptr != observer_) {
    observer_->HandleClose(session);
  }

  DeleteHttpParser();
}

void SessionObserver::RegisterObserver(Packet::Observer& observer) {
  observer_ = &observer;
}

void SessionObserver::CreateHttpParser() {
  if (nullptr == current_http_parser_) {
    current_http_parser_ = new http::Parser();
  }
}

void SessionObserver::DeleteHttpParser() {
  if (nullptr != current_http_parser_) {
    delete current_http_parser_;
    current_http_parser_ = nullptr;
  }
}

SessionObserverCreator::SessionObserverCreator(Packet::Observer& observer)
  : observer_(observer) {
}

SessionObserverCreator::~SessionObserverCreator() {
}

net::SessionObserver* SessionObserverCreator::Create() {
  SessionObserver* handler = new http::SessionObserver();
  handler->RegisterObserver(observer_);

  return handler;
}

void SessionObserverCreator::Delete(net::SessionObserver* handler) {
  delete handler;
}

}  // namespace http
}  // namespace anet
