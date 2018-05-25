#include <boost/bind.hpp>

#include "tcp/client.h"
#include "common/logger.h"

namespace tcp {


Client::Client(boost::asio::io_service& io_service)
    : io_service_(io_service) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

Client::~Client() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

SessionPtr Client::CreateSession(const impl::tcp::endpoint& target) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
  SessionPtr new_session(nullptr);

  auto accessor = GetNewSessionAccessor();
  if (nullptr != accessor) {
    new_session = SessionPtr(new Session(*accessor, io_service_));
    AddSession(new_session);

    new_session->GetSocket().connect(target);
  }

  return new_session;
}

} // namespace tcp
