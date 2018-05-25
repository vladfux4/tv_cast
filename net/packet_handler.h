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
   * @param session Pointer on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status Handle(SessionPtr session,
                        const boost::asio::const_buffer& buffer) = 0;

  /**
   * @brief Handle Close event
   *
   * @param session Pointer on session
   */
  virtual void HandleClose(SessionPtr session) = 0;
};

/**
 * @brief The Packet Handler Creator class
 */
class PacketHandlerCreator {
 public:
  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual PacketHandler* Create() = 0;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(PacketHandler* handler) = 0;

  /**
   * @brief Destructor
   */
  virtual ~PacketHandlerCreator() {}
};

} // namespace net

#endif  // NET_PACKET_HANDLER_H
