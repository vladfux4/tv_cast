#include "common/logger.h"

#include "com_ctrl.h"
#include "logic/logic.h"

int main(int argc, char* argv[]) {
  LOG(LogLevel::INFO) << "Run";

  try {
    CommunicationController com_ctr;
    logic::Logic logic;

    com_ctr.RegisterHttpObserver(logic);

    com_ctr.Run();
  } catch (std::exception& e) {
    LOG(LogLevel::DEBUG) << "Exception: " << e.what();
  }

  return 0;
}
