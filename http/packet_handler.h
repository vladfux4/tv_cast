#include "net/packet_handler.h"
#include "http/packet.h"
#include "http/parser.h"

namespace http {

class PacketHandler : public net::PacketHandler {
 public:
  /**
   * @brief Constructor
   */
  PacketHandler();

  /**
   * @brief Destructor
   */
  virtual ~PacketHandler();

  virtual Status Handle(net::SessionPtr session,
      const boost::asio::const_buffer& buffer) override;

  virtual void HandleClose(net::SessionPtr session) override;

  /**
   * @brief Register HTTP Observer
   *
   * @param observer Reference on observer
   */
  void RegisterObserver(http::Packet::Observer& observer);

 private:
  /**
   * @brief DeleteHttpParser
   */
  void CreateHttpParser();

  /**
   * @brief DeleteHttpParser
   */
  void DeleteHttpParser();

  http::Packet::Observer* observer_;
  http::Parser* current_http_parser_;
};

}  // namespace http
