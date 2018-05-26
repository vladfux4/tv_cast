#include "anet/net/session_handler.h"

#include "common/logger.h"

namespace anet {
namespace net {

SessionHandler::SessionHandler()
    : creator_(nullptr),
      session_pool_() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

SessionHandler::~SessionHandler() {
  DLOG(INFO) << __PRETTY_FUNCTION__;
}

void SessionHandler::RegisterCreator(SessionObserverCreator& creator) {
  creator_ = &creator;
}

void SessionHandler::CloseSession(const SessionAccessor& accessor) {
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

SessionHandler::SessionAccessorPtr SessionHandler::GetNewSessionAccessor() {
  SessionAccessorPtr retval(nullptr);
  static uint32_t index = 0;

  do {
    if (nullptr == creator_) {
      break;
    }

    net::SessionObserver* handler = creator_->Create();
    if (nullptr == handler) {
      break;
    }

    retval.reset(new SessionAccessor(*this, *handler, index++));
  } while (false);

  return boost::move(retval);
}

void SessionHandler::AddSession(SessionPtr session) {
  session_pool_.push_back(session);
}

} // namespace net
} // namespace anet
