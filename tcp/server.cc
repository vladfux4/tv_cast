#include <boost/bind.hpp>

#include "tcp/server.h"
#include "common/logger.h"

namespace tcp {

namespace impl = boost::asio::ip;

Server::Server(server::PacketHandler& packet_handler, const uint16_t port)
    : packet_handler_(packet_handler),
      acceptor_(io_service_, impl::tcp::endpoint(impl::tcp::v4(), port)),
      session_pool_() {
  LOG(LogLevel::DEBUG) << "New TCP Server on port:" << port;

  CreateSession();
}

Server::~Server() {
}

void Server::Run() {
  LOG(LogLevel::DEBUG) << "Run TCP Server";

  io_service_.run();
}

void Server::HandleAccept(SessionPtr new_session,
                          const boost::system::error_code& error) {
  LOG(LogLevel::DEBUG) << "Handle Accept";

  if (!error) {
    new_session->Start();
    CreateSession();
  } else {
    new_session->Close();
  }
}

server::PacketHandler::Status Server::Handle(
    server::Session& session,
    const boost::asio::const_buffer& buffer) {
  LOG(LogLevel::DEBUG) << "Handle packet buffer";

  return packet_handler_.Handle(session, buffer);
}

void Server::CloseSession(const ServerAccessor& accessor) {
  for(SessionPool::iterator it = session_pool_.begin();
      it != session_pool_.end(); ++it) {
    Session& session = *(*it);

    if (session.GetServerAccessor().kId == accessor.kId) {
      LOG(LogLevel::DEBUG) << "Close TCP session ID:"
                           << accessor.kId;

      session_pool_.erase(it);
      break;
    }
  }
}

void Server::CreateSession() {
  static uint32_t index = 0;
  SessionPtr new_session(
      new Session(ServerAccessor(*this, index++), io_service_));
  session_pool_.push_back(new_session);

  acceptor_.async_accept(new_session->GetSocket(),
      boost::bind(&Server::HandleAccept, this, new_session,
          boost::asio::placeholders::error));
}

} // namespace tcp
