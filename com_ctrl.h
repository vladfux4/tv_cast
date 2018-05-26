#include <boost/move/unique_ptr.hpp>
#include <boost/asio.hpp>

#include "anet/tcp/server.h"
#include "anet/tcp/client.h"
#include "anet/http/session_observer.h"

#include "anet/udp/server.h"

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

  void RegisterHttpServerObserver(anet::http::Packet::Observer& observer);
  void RegisterHttpClientObserver(anet::http::Packet::Observer& observer);
  anet::tcp::SessionPtr CreateHttpClientSession(
      const boost::asio::ip::tcp::endpoint& target);

  /**
   * @brief Run
   */
  void Run();

 private:
  boost::asio::io_service io_service_;
  anet::tcp::Server tcp_server_;
  anet::tcp::Client tcp_client_;
  boost::movelib::unique_ptr<anet::http::SessionObserverCreator>
      http_handler_creator_;
  boost::movelib::unique_ptr<anet::http::SessionObserverCreator>
      http_client_handler_creator_;
  anet::udp::Server udp_server_;
};
