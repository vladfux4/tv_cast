#ifndef LOGIC_LOGIC_H
#define LOGIC_LOGIC_H

#include "http/packet.h"

namespace logic {

class Logic : public http::Packet::Observer {
 public:
  /**
   * @brief Logic
   */
  Logic();

  /**
   * @brief Handle HTTP Packet
   *
   * @param session Server session
   * @param packet Packet
   */
  virtual void HandlePacket(net::SessionPtr session,
                            const http::Packet& packet) override;

  /**
   * @brief Handle Close event
   *
   * @param session Pointer on session
   */
  virtual void HandleClose(net::SessionPtr session) override;
};

}  // namespace logic

#endif  //LOGIC_LOGIC_H
