#include "common/logger.h"

#include "com_ctrl.h"
#include "app_ctrl.h"

int main(int argc, char* argv[]) {
  InitLogger(argv[0]);
  LOG(INFO) << "Start";

  try {
    CommunicationController com_ctr;
    ApplicationController app_ctrl;
    com_ctr.RegisterAppController(app_ctrl);

    com_ctr.Run();
  } catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }

  return 0;
}
