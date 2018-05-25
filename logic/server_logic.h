#ifndef LOGIC_SERVER_LOGIC_H
#define LOGIC_SERVER_LOGIC_H

#include "http/packet.h"

namespace logic {

class ServerLogic : public http::Packet::Observer {
 public:
  /**
   * @brief Logic
   */
  ServerLogic();

  virtual void HandlePacket(net::SessionPtr session,
                            const http::Packet& packet) override;
  virtual void HandleWriteComplete(net::SessionPtr session) override;
  virtual void HandleClose(net::SessionPtr session) override;
};

}  // namespace logic

#endif  // LOGIC_SERVER_LOGIC_H
