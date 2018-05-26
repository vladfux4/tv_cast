#ifndef APP_CTRL_H
#define APP_CTRL_H

#include "anet/net/packet_observer.h"
#include "anet/http/packet.h"

class ApplicationController {
 public:
  ApplicationController();

  virtual ~ApplicationController();

  virtual void HandleUdpData(const boost::asio::const_buffer& buffer);
  virtual void NotifyUdpDataSend();

  virtual void HandleServicePacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet);
  virtual void NotifyServiceDataSend(anet::net::SessionPtr session);
  virtual void NotifyServiceSessionClose(anet::net::SessionPtr session);

  virtual void HandleLoopPacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet);
  virtual void NotifyLoopDataSend(anet::net::SessionPtr session);
  virtual void NotifyLoopSessionClose(anet::net::SessionPtr session);

};

#endif  // APP_CTRL_H
