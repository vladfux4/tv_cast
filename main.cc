#include <gflags/gflags.h>

#include "common/logger.h"

#include "com_ctrl.h"
#include "app_ctrl.h"

DEFINE_string(video, "",
              "URL on video you want to cast");

static bool IsNonEmptyMessage(const char *flagname, const std::string &value) {
  return value[0] != '\0';
}

DEFINE_validator(video, &IsNonEmptyMessage);

int main(int argc, char* argv[]) {
  gflags::SetUsageMessage("TV Cast APP");
  gflags::SetVersionString("0.0.1");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::string url = FLAGS_video;

  gflags::ShutDownCommandLineFlags();

  InitLogger(argv[0]);

  try {
    boost::asio::io_service io_service;
    CommunicationController com_ctr(io_service);
    ApplicationController app_ctrl(com_ctr);
    com_ctr.RegisterAppController(app_ctrl);
    com_ctr.Start();

    app_ctrl.PlayVideo(url);

    io_service.run();
  } catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }

  return 0;
}
