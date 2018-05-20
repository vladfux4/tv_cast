#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"
#include "http/parser.h"

CommunicationController::CommunicationController()
  : tcp_(*this, 49080),
    current_http_parser_(nullptr) {
}

CommunicationController::~CommunicationController() {
  DeleteHttpParser();
}

server::PacketHandler::Status CommunicationController::Handle(
    server::Session& session,
    const boost::asio::const_buffer& buffer) {
  Status status = Status::NOT_SUPPORTED;

  do {
    if (0 == buffer.size()) {
      DeleteHttpParser();
      break;
    }

    CreateHttpParser();

    status = current_http_parser_->Parse(buffer);

    if (Status::ERROR == status) {
      DeleteHttpParser();
      session.Close();
      break;
    }

    if (Status::PART_RECEIVED == status) {
      LOG(LogLevel::DEBUG) << "Waiting for rest of package";
      break;
    }

    if (Status::OK == status) {
      http::Parser::PacketPtr packet = current_http_parser_->GetPacket();
      for (http::Packet::Observer* observer : http_observers_) {
        observer->HandlePacket(session, *packet);
      }

      DeleteHttpParser();
    }
  } while (false);

  return status;
}

void CommunicationController::RegisterHttpObserver(
    http::Packet::Observer& observer) {
 http_observers_.push_back(&observer);
}

void CommunicationController::Run() {
  tcp_.Run();
}

void CommunicationController::CreateHttpParser() {
  if (nullptr == current_http_parser_) {
    current_http_parser_ = new http::Parser();
  }
}

void CommunicationController::DeleteHttpParser() {
  if (nullptr != current_http_parser_) {
    delete current_http_parser_;
    current_http_parser_ = nullptr;
  }
}
