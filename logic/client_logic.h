#ifndef LOGIC_CLIENT_LOGIC_H
#define LOGIC_CLIENT_LOGIC_H

#include "anet/http/packet.h"

class CommunicationController;

namespace logic {

class ClientLogic : public anet::http::Packet::Observer {
 public:
  /**
   * @brief Logic
   */
  ClientLogic(CommunicationController& com_ctrl);

  //test function
  void SendGetRequest();

  virtual void HandlePacket(anet::net::SessionPtr session,
                            const anet::http::Packet& packet) override;
  virtual void HandleWriteComplete(anet::net::SessionPtr session) override;
  virtual void HandleClose(anet::net::SessionPtr session) override;

 private:
  CommunicationController& com_ctrl_;
};

}  // namespace logic

#endif  // LOGIC_CLIENT_LOGIC_H
