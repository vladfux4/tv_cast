#ifndef TCP_SERVER_ACCESSOR_H
#define TCP_SERVER_ACCESSOR_H

#include <stdint.h>

namespace tcp {

class Server;

/**
 * @brief The Accessor to server for session
 */
struct ServerAccessor {
  /**
   * @brief Contructor
   *
   * @param Reference on TCP server
   * @param kId Session ID
   */
  ServerAccessor(Server& server,
                 const uint32_t kId)
      : server(server), kId(kId) {}

  Server& server;
  const uint32_t kId;
};

} // namespace tcp

#endif  // TCP_SERVER_ACCESSOR_HT
