#include <sstream>
#include "common/logger.h"

#include "com_ctrl.h"

CommunicationController::CommunicationController()
  : io_service_(),
    tcp_server_(io_service_, 8080),
    tcp_client_(io_service_),
    http_handler_creator_(nullptr),
    http_client_handler_creator_(nullptr) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

CommunicationController::~CommunicationController() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

void CommunicationController::RegisterHttpServerObserver(
    http::Packet::Observer& observer) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  http_handler_creator_.reset(new http::PacketHandlerCreator(observer));
  tcp_server_.RegisterCreator(*http_handler_creator_);
}

void CommunicationController::RegisterHttpClientObserver(
    http::Packet::Observer& observer) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  http_client_handler_creator_.reset(new http::PacketHandlerCreator(observer));
  tcp_client_.RegisterCreator(*http_client_handler_creator_);
}

tcp::SessionPtr CommunicationController::CreateHttpClientSession(
    const boost::asio::ip::tcp::endpoint& target) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  return tcp_client_.CreateSession(target);
}

void CommunicationController::Run() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  tcp_server_.Start();

  io_service_.run();
}
