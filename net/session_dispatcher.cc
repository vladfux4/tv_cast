#include "net/session_dispatcher.h"

#include "common/logger.h"

namespace net {

SessionDispatcher::SessionDispatcher()
    : creator_(nullptr),
      session_pool_() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

SessionDispatcher::~SessionDispatcher() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void SessionDispatcher::RegisterCreator(SessionHandlerCreator& creator) {
  creator_ = &creator;
}

void SessionDispatcher::CloseSession(const SessionAccessor& accessor) {
  for(SessionPool::iterator it = session_pool_.begin();
      it != session_pool_.end(); ++it) {
    Session& session = *(*it);

    if (session.GetAccessor().kId == accessor.kId) {
      DLOG(INFO) << "Close TCP session ID:"
                           << accessor.kId;
      if (nullptr != creator_) {
        creator_->Delete(&(session.GetAccessor().handler));
      }

      session_pool_.erase(it);
      break;
    }
  }
}

SessionDispatcher::SessionAccessorPtr SessionDispatcher::GetNewSessionAccessor() {
  SessionAccessorPtr retval(nullptr);
  static uint32_t index = 0;

  do {
    if (nullptr == creator_) {
      break;
    }

    net::SessionHandler* handler = creator_->Create();
    if (nullptr == handler) {
      break;
    }

    retval.reset(new SessionAccessor(*this, *handler, index++));
  } while (false);

  return boost::move(retval);
}

void SessionDispatcher::AddSession(SessionPtr session) {
  session_pool_.push_back(session);
}

} // namespace net
