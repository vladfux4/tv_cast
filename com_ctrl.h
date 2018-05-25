#include <boost/move/unique_ptr.hpp>
#include <boost/asio.hpp>

#include "tcp/server.h"
#include "tcp/client.h"
#include "http/packet_handler.h"

class CommunicationController {
 public:
  /**
   * @brief Constructor
   */
  CommunicationController();

  /**
   * @brief Destructor
   */
  virtual ~CommunicationController();

  void RegisterHttpServerObserver(http::Packet::Observer& observer);
  void RegisterHttpClientObserver(http::Packet::Observer& observer);
  tcp::SessionPtr CreateHttpClientSession(
      const boost::asio::ip::tcp::endpoint& target);

  /**
   * @brief Run
   */
  void Run();

 private:
  boost::asio::io_service io_service_;
  tcp::Server tcp_server_;
  tcp::Client tcp_client_;
  boost::movelib::unique_ptr<http::PacketHandlerCreator> http_handler_creator_;
  boost::movelib::unique_ptr<http::PacketHandlerCreator> http_client_handler_creator_;
};
