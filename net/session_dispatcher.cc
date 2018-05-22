#include "net/session_dispatcher.h"

#include "common/logger.h"

namespace net {

SessionDispatcher::SessionDispatcher()
    : packet_handler_(nullptr),
      session_pool_() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

SessionDispatcher::~SessionDispatcher() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

void SessionDispatcher::RegisterHandler(net::PacketHandler& handler) {
  packet_handler_ = &handler;
}

net::PacketHandler::Status SessionDispatcher::Handle(
    net::Session& session,
    const boost::asio::const_buffer& buffer) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  return packet_handler_->Handle(session, buffer);
}

void SessionDispatcher::CloseSession(const SessionAccessor& accessor) {
  for(SessionPool::iterator it = session_pool_.begin();
      it != session_pool_.end(); ++it) {
    Session& session = *(*it);

    if (session.GetAccessor().kId == accessor.kId) {
      LOG(LogLevel::DEBUG) << "Close TCP session ID:"
                           << accessor.kId;

      session_pool_.erase(it);
      break;
    }
  }
}

const SessionAccessor SessionDispatcher::GetNewSessionAccessor() {
  static uint32_t index = 0;
  return SessionAccessor(*this, index++);
}

void SessionDispatcher::AddSession(SessionPtr session) {
  session_pool_.push_back(session);
}


} // namespace net
