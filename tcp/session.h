#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <stdint.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "net/session.h"

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
  virtual void Write(const boost::asio::const_buffer& buffer) override;

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
  void HandleWrite(const boost::system::error_code& error);

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

} // namespace net

#endif  // TCP_SESSION_H
