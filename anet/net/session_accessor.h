#ifndef NET_SESSION_ACCESSOR_H
#define NET_SESSION_ACCESSOR_H

#include <stdint.h>

namespace anet {
namespace net {

class SessionDispatcher;
class SessionHandler;

/**
 * @brief The Session Accessor struct
 */
struct SessionAccessor {
  /**
   * @brief SessionAccessor
   *
   * @param dispatcher Reference on session dispatcher
   * @param dispatcher Reference on Packet handler
   * @param kId Dispatcher internal id
   */
  SessionAccessor(SessionDispatcher& dispatcher,
                  SessionHandler& handler,
                 const uint32_t kId)
      : dispatcher(dispatcher), handler(handler), kId(kId) {}

  SessionDispatcher& dispatcher;
  SessionHandler& handler;
  const uint32_t kId;
};

} // namespace net
} // namespace anet

#endif  // NET_SESSION_ACCESSOR_H
