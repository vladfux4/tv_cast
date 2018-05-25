#ifndef HTTP_SESSION_HANDLER_H
#define HTTP_SESSION_HANDLER_H

#include "anet/net/session_handler.h"
#include "anet/http/packet.h"
#include "anet/http/parser.h"

namespace anet {
namespace http {

class SessionHandler : public net::SessionHandler {
 public:
  /**
   * @brief Constructor
   */
  SessionHandler();

  /**
   * @brief Destructor
   */
  virtual ~SessionHandler();

  ///net::SessionHandler interface
  virtual Status HandleData(net::SessionPtr session,
      const boost::asio::const_buffer& buffer) override;
  virtual void HandleWriteComplete(net::SessionPtr session) override;
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
class SessionHandlerCreator : public net::SessionHandlerCreator {
 public:
 /**
   * @brief Constructor
   *
   * @param observer Reference HTTP Packet observer
   */
  SessionHandlerCreator(http::Packet::Observer& observer);

  /**
   * @brief Destructor
   */
  virtual ~SessionHandlerCreator();

  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual net::SessionHandler* Create() override;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(net::SessionHandler* handler) override;

 private:
  http::Packet::Observer& observer_;
};

}  // namespace http
}  // namespace anet

#endif  // HTTP_SESSION_HANDLER_H
