#ifndef NET_SESSION_H
#define NET_SESSION_H

#include <boost/asio/buffer.hpp>

#include "net/session_accessor.h"

namespace net {

/**
 * @brief The Session class
 */
class Session {
 public:
  /**
   * @brief Constructor
   *
   * @param accessor Session accessor
   */
  Session(const net::SessionAccessor& accessor)
    : accessor_(accessor) {}

  /**
   * @brief Destructor
   */
  virtual ~Session() {}

  /**
   * @brief Start async read operation
   */
  virtual void Read() = 0;

  /**
   * @brief Start async write operation
   */
  virtual void Write(const boost::asio::const_buffer& buffer) = 0;

  /**
   * @brief Close session
   */
  virtual void Close() = 0;

  /**
   * @brief Get Server Accessor
   *
   * @return Server Accessor
   */
  inline const net::SessionAccessor& GetAccessor() const;

 protected:
  net::SessionAccessor accessor_;
};

inline const net::SessionAccessor& Session::GetAccessor() const {
  return accessor_;
}

} // namespace net

#endif // NET_SESSION_H
