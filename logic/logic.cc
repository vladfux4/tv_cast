#include "logic/logic.h"
#include "common/logger.h"

namespace logic {

Logic::Logic() {
}

void Logic::HandlePacket(server::Session& session,
                         const http::Packet& packet) {
  LOG(LogLevel::INFO) << "LOGIC TEST";
}

}  // namespace logic
