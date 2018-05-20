#ifndef HTTP_PACKET_H
#define HTTP_PACKET_H

#include "server/session.h"

namespace http {

/**
 * @brief The HTTP Packet class
 */
class Packet {
 public:
  /**
   * @brief The HTTP Observer class
   */
  class Observer {
   public:
  /**
     * @brief Handle HTTP Packet
     *
     * @param session Server session
     * @param packet Packet
     */
    virtual void HandlePacket(server::Session& session,
                              const Packet& packet) = 0;
  };

  Packet() {}
};

}  // namespace http

#endif  //HTTP_PACKET_H
