#include <boost/bind.hpp>

#include "tcp/client.h"
#include "common/logger.h"

namespace tcp {


Client::Client(boost::asio::io_service& io_service)
    : io_service_(io_service) {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

Client::~Client() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

SessionPtr Client::CreateSession(const impl::tcp::endpoint& target) {
  auto accessor = GetNewSessionAccessor();
  CHECK_NOTNULL(accessor);

  SessionPtr new_session = SessionPtr(new Session(*accessor, io_service_));
  AddSession(new_session);

  new_session->GetSocket().connect(target);

  return new_session;
}

} // namespace tcp
