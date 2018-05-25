#ifndef HTTP_PACKET_HANDLER_H
#define HTTP_PACKET_HANDLER_H

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

  ///net::PacketHandler interface
  virtual Status Handle(net::SessionPtr session,
      const boost::asio::const_buffer& buffer) override;

  ///net::PacketHandler interface
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

/**
 * @brief The Packet Handler Creator class
 */
class PacketHandlerCreator : public net::PacketHandlerCreator {
 public:
 /**
   * @brief Constructor
   *
   * @param observer Reference HTTP Packet observer
   */
  PacketHandlerCreator(http::Packet::Observer& observer);

  /**
   * @brief Destructor
   */
  virtual ~PacketHandlerCreator();

  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual net::PacketHandler* Create() override;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(net::PacketHandler* handler) override;

 private:
  http::Packet::Observer& observer_;
};

}  // namespace http

#endif  // HTTP_PACKET_HANDLER_H
