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

  /**
   * @brief Register HTTP Observer
   *
   * @param observer Reference on observer
   */
  void RegisterHttpObserver(http::Packet::Observer& observer);

  /**
   * @brief Run
   */
  void Run();

 private:
  boost::asio::io_service io_service_;
  tcp::Server tcp_server_;
  boost::movelib::unique_ptr<http::PacketHandlerCreator> http_handler_creator_;
};
