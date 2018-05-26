#ifndef HTTP_SESSION_HANDLER_H
#define HTTP_SESSION_HANDLER_H

#include "anet/net/session_observer.h"
#include "anet/http/packet.h"
#include "anet/http/parser.h"

namespace anet {
namespace http {

class SessionObserver : public net::SessionObserver {
 public:
  /**
   * @brief Constructor
   */
  SessionObserver();

  /**
   * @brief Destructor
   */
  virtual ~SessionObserver();

  ///net::SessionObserver interface
  virtual net::PacketObserver::Status HandleData(net::SessionPtr session,
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
class SessionObserverCreator : public net::SessionObserverCreator {
 public:
 /**
   * @brief Constructor
   *
   * @param observer Reference HTTP Packet observer
   */
  SessionObserverCreator(http::Packet::Observer& observer);

  /**
   * @brief Destructor
   */
  virtual ~SessionObserverCreator();

  /**
   * @brief Create new packet handler
   *
   * @return pointer on handler
   */
  virtual net::SessionObserver* Create() override;

  /**
   * @brief Delete packet handler
   *
   * @param pointer on handler
   */
  virtual void Delete(net::SessionObserver* handler) override;

 private:
  http::Packet::Observer& observer_;
};

}  // namespace http
}  // namespace anet

#endif  // HTTP_SESSION_HANDLER_H
