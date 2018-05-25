#ifndef LOGIC_SERVER_LOGIC_H
#define LOGIC_SERVER_LOGIC_H

#include "anet/http/packet.h"

namespace logic {

class ServerLogic : public anet::http::Packet::Observer {
 public:
  /**
   * @brief Logic
   */
  ServerLogic();

  virtual void HandlePacket(anet::net::SessionPtr session,
                            const anet::http::Packet& packet) override;
  virtual void HandleWriteComplete(anet::net::SessionPtr session) override;
  virtual void HandleClose(anet::net::SessionPtr session) override;
};

}  // namespace logic

#endif  // LOGIC_SERVER_LOGIC_H
