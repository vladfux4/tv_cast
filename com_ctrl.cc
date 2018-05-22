#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"
#include "http/parser.h"

CommunicationController::CommunicationController()
  : io_service_(),
    tcp_(io_service_, 49080),
    http_() {
  tcp_.RegisterHandler(http_);
}

CommunicationController::~CommunicationController() {
}

void CommunicationController::RegisterHttpObserver(
    http::Packet::Observer& observer) {
  http_.RegisterObserver(observer);
}

void CommunicationController::Run() {
  io_service_.run();
}
