#ifndef NET_SESSION_HANDLER_H
#define NET_SESSION_HANDLER_H

#include <boost/asio/buffer.hpp>
#include "net/session.h"

namespace net {

/**
 * @brief The SessionHandler Interface class
 */
class SessionHandler {
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
  virtual ~SessionHandler() {}

  /**
   * @brief Handle packet
   *
   * @param session Pointer on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status HandleData(SessionPtr session,
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
class SessionHandlerCreator {
 public:
  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual SessionHandler* Create() = 0;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(SessionHandler* handler) = 0;

  /**
   * @brief Destructor
   */
  virtual ~SessionHandlerCreator() {}
};

} // namespace net

#endif  // NET_SESSION_HANDLER_H
