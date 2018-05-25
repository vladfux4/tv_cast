#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"

CommunicationController::CommunicationController()
  : io_service_(),
    tcp_server_(io_service_, 49080),
    http_handler_creator_(nullptr) {
}

CommunicationController::~CommunicationController() {
}

void CommunicationController::RegisterHttpObserver(
    http::Packet::Observer& observer) {

  http_handler_creator_.reset(new http::PacketHandlerCreator(observer));
  tcp_server_.RegisterCreator(*http_handler_creator_);
}

void CommunicationController::Run() {
  tcp_server_.Start();

  io_service_.run();
}
