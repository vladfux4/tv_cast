#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H

#include <boost/asio/buffer.hpp>

namespace server {

/**
 * @brief The Session Interface class
 */
class Session {
 public:
  /**
   * @brief Start async write operation
   */
  virtual void Write(const boost::asio::const_buffer& buffer) = 0;

  /**
   * @brief Close session
   */
  virtual void Close() = 0;

  /**
   * @brief Destructor
   */
  virtual ~Session() {}
};

} // namespace server

#endif // SERVER_SESSION_H
