#ifndef NET_SESSION_OBSERVER_H
#define NET_SESSION_OBSERVER_H

#include <boost/asio/buffer.hpp>

#include "anet/net/session.h"
#include "anet/net/packet_observer.h"

namespace anet {
namespace net {

/**
 * @brief The SessionObserver Interface class
 */
class SessionObserver {
 public:

  /**
   * @brief Destructor
   */
  virtual ~SessionObserver() {}

  /**
   * @brief Handle packet
   *
   * @param session Pointer on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual PacketObserver::Status HandleData(SessionPtr session,
      const boost::asio::const_buffer& buffer) = 0;

  /**
   * @brief Handle Close event
   *
   * @param session Pointer on session
   */
  virtual void HandleWriteComplete(SessionPtr session) = 0;

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
class SessionObserverCreator {
 public:
  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual SessionObserver* Create() = 0;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(SessionObserver* handler) = 0;

  /**
   * @brief Destructor
   */
  virtual ~SessionObserverCreator() {}
};

} // namespace net
} // namespace anet

#endif  // NET_SESSION_OBSERVER_H
