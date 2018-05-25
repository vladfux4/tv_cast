#ifndef NET_SESSION_H
#define NET_SESSION_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/buffer.hpp>

#include "anet/net/session_accessor.h"

namespace anet {
namespace net {

/**
 * @brief The Session class
 */
class Session : public boost::enable_shared_from_this<Session> {
 public:
  /**
   * @brief The Session state enum
   */
  enum class State {
    INIT,
    RUN,
    CLOSED
  };

  /**
   * @brief Byte buffer type
   */
  typedef std::vector<uint8_t> Buffer;
  typedef boost::shared_ptr<Buffer> BufferPtr;

  /**
   * @brief Constructor
   *
   * @param accessor Session accessor
   */
  Session(const net::SessionAccessor& accessor)
    : accessor_(accessor), state_(State::INIT) {}

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
  virtual void Write(BufferPtr buffer) = 0;

  /**
   * @brief Close session
   */
  virtual void Close() = 0;

  /**
   * @brief Get Session accessor
   *
   * @return Session accessor
   */
  inline const net::SessionAccessor& GetAccessor() const;

  /**
   * @brief Get Session state
   *
   * @return state
   */
  inline const State& GetState() const;

 protected:
  net::SessionAccessor accessor_;
  State state_;
};

inline const net::SessionAccessor& Session::GetAccessor() const {
  return accessor_;
}

inline const Session::State& Session::GetState() const {
  return state_;
}

/**
 * @brief Session pointer
 */
typedef boost::shared_ptr<Session> SessionPtr;

} // namespace net
} // namespace anet

#endif // NET_SESSION_H
