#ifndef LOGIC_CLIENT_LOGIC_H
#define LOGIC_CLIENT_LOGIC_H

#include "http/packet.h"

class CommunicationController;

namespace logic {

class ClientLogic : public http::Packet::Observer {
 public:
  /**
   * @brief Logic
   */
  ClientLogic(CommunicationController& com_ctrl);

  //test function
  void SendGetRequest();

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

 private:
  CommunicationController& com_ctrl_;
};

}  // namespace logic

#endif  // LOGIC_CLIENT_LOGIC_H
