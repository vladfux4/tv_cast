#include <boost/container/vector.hpp>

#include "server/packet_handler.h"
#include "tcp/server.h"
#include "http/packet.h"
#include "http/parser.h"

class CommunicationController : public server::PacketHandler {
 public:
  /**
   * @brief Constructor
   */
  CommunicationController();

  /**
   * @brief Destructor
   */
  virtual ~CommunicationController();

  virtual Status Handle(server::Session& session,
                        const boost::asio::const_buffer& buffer);

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

  /**
   * @brief DeleteHttpParser
   */
  void CreateHttpParser();

  /**
   * @brief DeleteHttpParser
   */
  void DeleteHttpParser();

  tcp::Server tcp_;
  boost::container::vector<http::Packet::Observer*> http_observers_;
  http::Parser* current_http_parser_;
};
