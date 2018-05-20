#include "logger.h"

//log level is is changing here
LogLevel LOG::kCurrentLogLevel = LogLevel::DEBUG;

LOG::LOG()
    : opened_(false),
      kMsgLevel(LogLevel::DEBUG) {}

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

