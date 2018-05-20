#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <stdint.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "server/session.h"
#include "tcp/server_accessor.h"

namespace tcp {

/**
 * @brief The TCP Session class
 */
class Session : public server::Session {
 public:
  /**
   * @brief Constructor
   *
   * @param Server accessor
   * @param io_service ASIO IO Service
   */
  Session(const ServerAccessor& accessor, boost::asio::io_service& io_service);

  /**
   * @brief Destructor
   */
  virtual ~Session();

  /**
   * @brief Start session
   */
  void Start();

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
   * @param error
   * @param bytes_transferred
   */
  void HandleRead(const boost::system::error_code& error,
                  size_t bytes_transferred);

  /**
   * @brief Handle write to socket operation completed
   *
   * @param error
   */
  void HandleWrite(const boost::system::error_code& error);

  /**
   * @brief Get Socket
   *
   * @return Socket
   */
  inline boost::asio::ip::tcp::socket& GetSocket();

  /**
   * @brief Get Server Accessor
   *
   * @return Server Accessor
   */
  inline const ServerAccessor& GetServerAccessor() const;

 private:
  /**
   * @brief Start async read operation
   */
  void Read();

  /**
   * @brief Destroy session
   */
  void Destroy();

  ServerAccessor server_accessor_;
  boost::asio::ip::tcp::socket socket_;
  boost::array<uint8_t, 2048> buffer_;
};

inline boost::asio::ip::tcp::socket& Session::GetSocket() {
  return socket_;
}

inline const ServerAccessor& Session::GetServerAccessor() const {
  return server_accessor_;
}

} // namespace server

#endif  // TCP_SESSION_H
