#ifndef NET_SESSION_ACCESSOR_H
#define NET_SESSION_ACCESSOR_H

#include <stdint.h>

namespace net {

class SessionDispatcher;

/**
 * @brief The Session Accessor struct
 */
struct SessionAccessor {
  /**
   * @brief SessionAccessor
   *
   * @param dispatcher Reference on session dispatcher
   * @param kId Dispatcher internal id
   */
  SessionAccessor(SessionDispatcher& dispatcher,
                 const uint32_t kId)
      : dispatcher(dispatcher), kId(kId) {}

  SessionDispatcher& dispatcher;
  const uint32_t kId;
};

} // namespace net

#endif  // NET_SESSION_ACCESSOR_H
