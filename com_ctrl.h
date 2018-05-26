#include <boost/move/unique_ptr.hpp>
#include <boost/asio.hpp>

#include "anet/tcp/server.h"
#include "anet/tcp/client.h"
#include "anet/http/session_observer.h"

#include "anet/udp/server.h"

class ApplicationController;
class CommunicationController;

class UdpServerBridge : public anet::net::PacketObserver {
 public:
  UdpServerBridge(ApplicationController& ctrl) : ctrl_(ctrl) {}
  virtual Status HandleData(const boost::asio::const_buffer& buffer);
  virtual void HandleWriteComplete();
 private:
  ApplicationController& ctrl_;
};

class HttpServerBridge : public anet::http::Packet::Observer {
 public:
  HttpServerBridge(ApplicationController& ctrl) : ctrl_(ctrl) {}
  virtual void HandlePacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet) override;
  virtual void HandleWriteComplete(anet::net::SessionPtr session) override;
  virtual void HandleClose(anet::net::SessionPtr session) override;
 private:
  ApplicationController& ctrl_;
};

class HttpClientBridge : public anet::http::Packet::Observer {
 public:
  HttpClientBridge(ApplicationController& ctrl) : ctrl_(ctrl) {}
  virtual void HandlePacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet) override;
  virtual void HandleWriteComplete(anet::net::SessionPtr session) override;
  virtual void HandleClose(anet::net::SessionPtr session) override;
 private:
  ApplicationController& ctrl_;
};

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

  anet::tcp::SessionPtr CreateHttpClientSession(
      const boost::asio::ip::tcp::endpoint& target);

  void RegisterAppController(ApplicationController& ctrl);

  /**
   * @brief Run
   */
  void Run();

 private:
  boost::asio::io_service io_service_;
  anet::udp::Server udp_server_;
  anet::tcp::Client tcp_client_;
  anet::tcp::Server tcp_server_;
  boost::movelib::unique_ptr<UdpServerBridge> udp_server_bridge_;
  boost::movelib::unique_ptr<HttpClientBridge> http_client_bridge_;
  boost::movelib::unique_ptr<HttpServerBridge> http_server_bridge_;
  boost::movelib::unique_ptr<anet::http::SessionObserverCreator>
      http_client_observer_creator_;
  boost::movelib::unique_ptr<anet::http::SessionObserverCreator>
      http_server_observer_creator_;
};
