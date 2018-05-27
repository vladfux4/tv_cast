#include "common/logger.h"

#include "com_ctrl.h"
#include "app_ctrl.h"

int main(int argc, char* argv[]) {
  InitLogger(argv[0]);
  LOG(INFO) << "Start";

  try {
    boost::asio::io_service io_service;
    CommunicationController com_ctr(io_service);
    ApplicationController app_ctrl(com_ctr);
    com_ctr.RegisterAppController(app_ctrl);
    com_ctr.Start();

    app_ctrl.PlayVideo("http://2kit.de/cast/demovideo.mp4");

    io_service.run();
  } catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }

  return 0;
}
