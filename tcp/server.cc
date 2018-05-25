#include <boost/bind.hpp>

#include "tcp/server.h"
#include "common/logger.h"

namespace tcp {

namespace impl = boost::asio::ip;

Server::Server(boost::asio::io_service& io_service,
               const uint16_t port)
    : io_service_(io_service),
      acceptor_(io_service,
          impl::tcp::endpoint(impl::tcp::v4(), port)) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__ << "(" << port << ")";
}

Server::~Server() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

void Server::Start() {
  CreateSession();
}

void Server::HandleAccept(SessionPtr new_session,
                          const boost::system::error_code& error) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  if (!error) {
    new_session->Read();
    CreateSession();
  } else {
    new_session->Close();
  }
}

void Server::CreateSession() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  auto accessor = GetNewSessionAccessor();
  if (nullptr != accessor) {
    SessionPtr new_session(new Session(*accessor, io_service_));
    AddSession(new_session);

    acceptor_.async_accept(new_session->GetSocket(),
        boost::bind(&Server::HandleAccept, this, new_session,
            boost::asio::placeholders::error));
  }
}

} // namespace tcp
