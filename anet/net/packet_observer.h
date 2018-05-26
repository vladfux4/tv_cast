#ifndef NET_PACKET_OBSERVER_H
#define NET_PACKET_OBSERVER_H

#include <boost/asio/buffer.hpp>
#include "anet/net/session.h"

namespace anet {
namespace net {

/**
 * @brief The PacketObserver Interface class
 */
class PacketObserver {
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
  virtual ~PacketObserver() {}

  /**
   * @brief Handle packet
   *
   * @param session Pointer on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status HandleData(const boost::asio::const_buffer& buffer) = 0;

  /**
   * @brief Handle Close event
   *
   * @param session Pointer on session
   */
  virtual void HandleWriteComplete() = 0;
};

} // namespace net
} // namespace anet

#endif  // NET_PACKET_OBSERVER_H
