#ifndef NET_PACKET_HANDLER_H
#define NET_PACKET_HANDLER_H

#include <boost/asio/buffer.hpp>
#include "net/session.h"

namespace net {

/**
 * @brief The PacketHandler Interface class
 */
class PacketHandler {
 public:
  /**
   * @brief The handle status enum
   */
  enum class Status {
    OK,
    PART_RECEIVED,
    ERROR
  };

  /**
   * @brief Destructor
   */
  virtual ~PacketHandler() {}

  /**
   * @brief Handle packet
   *
   * @param session Reference on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status Handle(Session& session,
                        const boost::asio::const_buffer& buffer) = 0;
};

} // namespace net

#endif  // NET_PACKET_HANDLER_H
