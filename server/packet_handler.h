#ifndef SERVER_PACKET_HANDLER_H
#define SERVER_PACKET_HANDLER_H

#include <boost/asio/buffer.hpp>
#include "server/session.h"

namespace server {

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
    NOT_SUPPORTED,
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

} // namespace server

#endif  /* SERVER_PACKET_HANDLER_H */
