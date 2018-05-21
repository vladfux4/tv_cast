#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>

#include "server/packet_handler.h"
#include "tcp/session.h"
#include "tcp/server_accessor.h"

namespace tcp {

class Server : server::PacketHandler {
 public:
  /**
   * @brief Session pointer
   */
  typedef boost::shared_ptr<Session> SessionPtr;

  /**
   * @brief Constructor
   *
   * @param packet_handler Reference on packet handler
   * @param port TCP connection local port
   */
  Server(const uint16_t port);

  /**
   * @brief Destructor
   */
  virtual ~Server();

  /**
   * @brief Register TCP packets handler
   *
   * @param packet_handler Reference on handler
   */
  void RegisterHandler(server::PacketHandler& handler);

  /**
   * @brief Run server
   */
  void Run();

  /**
   * @brief Handle TCP accept
   *
   * @param new_session TCP Session
   * @param error Error code
   */
  void HandleAccept(SessionPtr new_session,
                    const boost::system::error_code& error);

  /**
   * @brief Handle packet
   *
   * @param session Reference on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status Handle(server::Session& session,
      const boost::asio::const_buffer& buffer) override;

  /**
   * @brief Close Session
   *
   * @param accessor Session's server accessor
   */
  void CloseSession(const ServerAccessor& accessor);

 private:
  /**
   * @brief Session pool type
   */
  typedef boost::container::vector<SessionPtr> SessionPool;

  /**
   * @brief Create new TCP session
   */
  void CreateSession();

  server::PacketHandler* packet_handler_;
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  SessionPool session_pool_;
};

} // namespace tcp

#endif  // TCP_SERVER_H
