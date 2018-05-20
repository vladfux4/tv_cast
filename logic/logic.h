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
  virtual void HandlePacket(server::Session& session,
                            const http::Packet& packet) override;
};

}  // namespace logic

#endif  //LOGIC_LOGIC_H
