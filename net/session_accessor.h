#ifndef NET_SESSION_ACCESSOR_H
#define NET_SESSION_ACCESSOR_H

#include <stdint.h>

namespace net {

class SessionDispatcher;
class PacketHandler;

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
                  PacketHandler& handler,
                 const uint32_t kId)
      : dispatcher(dispatcher), handler(handler), kId(kId) {}

  SessionDispatcher& dispatcher;
  PacketHandler& handler;
  const uint32_t kId;
};

} // namespace net

#endif  // NET_SESSION_ACCESSOR_H
