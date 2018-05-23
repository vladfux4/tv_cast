#include "logic/logic.h"
#include "common/logger.h"

namespace logic {

Logic::Logic() {
}

static net::SessionPtr dummy;

void Logic::HandlePacket(net::SessionPtr session,
                         const http::Packet& packet) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;

  dummy = session;
}

void Logic::HandleClose(net::SessionPtr session) {
  LOG(LogLevel::INFO) << __PRETTY_FUNCTION__;

  if (dummy == session) {
    dummy.reset();
  }
}

}  // namespace logic
