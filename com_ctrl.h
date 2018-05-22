#include <boost/container/vector.hpp>
#include <boost/asio.hpp>

#include "tcp/server.h"
#include "http/packet.h"
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
  tcp::Server tcp_;
  http::PacketHandler http_;
};
