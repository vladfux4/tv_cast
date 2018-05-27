#include "logger.h"

#ifndef ENABLE_GLOG

//log level is is changing here
LogLevel LOG::kCurrentLogLevel = LogLevel::INFO;

LOG::LOG()
    : opened_(false),
      kMsgLevel(LogLevel::INFO) {}

LOG::LOG(LogLevel level)
    : kMsgLevel(level) {
  operator << ("["+GetLabel(level)+"] ");
}

LOG::~LOG() {
  if(opened_ && (kMsgLevel >= kCurrentLogLevel)) {
    std::cout << std::endl;
  }

  opened_ = false;
}

#endif  // ENABLE_GLOG

void InitLogger(const char* str) {
#ifdef ENABLE_GLOG
  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = 1;
  FLAGS_log_prefix = 0;
  FLAGS_v = 0;

  google::InitGoogleLogging(str);
#endif  // NO_GLOG
}
