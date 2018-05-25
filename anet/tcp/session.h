#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <stdint.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "anet/net/session.h"

namespace anet {
namespace tcp {

/**
 * @brief The TCP Session class
 */
class Session : public net::Session {
 public:
  /**
   * @brief Constructor
   *
   * @param Session dispatcher accessor
   * @param io_service ASIO IO Service
   */
  Session(const net::SessionAccessor& accessor,
          boost::asio::io_service& io_service);

  /**
   * @brief Destructor
   */
  virtual ~Session();

  /**
   * @brief Start async read operation
   */
  virtual void Read() override;

  /**
   * @brief Start async write operation
   */
  virtual void Write(BufferPtr buffer) override;

  /**
   * @brief Close session
   */
  virtual void Close() override;

  /**
   * @brief Handle read from socket operation completed
   *
   * @param error Error code
   * @param bytes_transferred Data length
   */
  void HandleRead(const boost::system::error_code& error,
                  size_t bytes_transferred);

  /**
   * @brief Handle write to socket operation completed
   *
   * @param error Error code
   */
  void HandleWrite(const boost::system::error_code& error,
                   net::Session::BufferPtr buffer);

  /**
   * @brief Get Socket
   *
   * @return Socket
   */
  inline boost::asio::ip::tcp::socket& GetSocket();

 private:
  /**
   * @brief Destroy session
   */
  void Destroy();

  boost::asio::ip::tcp::socket socket_;
  enum { MAX_BUFFER_SIZE = 2048 };
  boost::array<uint8_t, MAX_BUFFER_SIZE> buffer_;
};

inline boost::asio::ip::tcp::socket& Session::GetSocket() {
  return socket_;
}

/**
 * @brief Session pointer
 */
typedef boost::shared_ptr<tcp::Session> SessionPtr;

} // namespace net
} // namespace anet

#endif  // TCP_SESSION_H
