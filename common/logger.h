#ifndef COMMONG_LOGGER_H
#define COMMONG_LOGGER_H

#define ENABLE_GLOG

/**
 * @brief Init Logger
 *
 * @param str Init string for logger configuration
 */
void InitLogger(const char* str);

#ifdef ENABLE_GLOG
#include <glog/logging.h>
#else

#include <iostream>

enum LogLevel {
  INFO,
  WARNING,
  ERROR
};

class LOG {
 public:
  /**
   * @brief Constructor
   */
  LOG();

  /**
   * @brief Constructor
   *
   * @param Logging level
   */
  explicit LOG(LogLevel level);

  /**
   * @brief Destructor
   */
  ~LOG();

  /**
   * @brief << operator
   *
   * @param msg Message
   */
  template<class T>
  LOG& operator<<(const T& msg) {
    if(kMsgLevel >= kCurrentLogLevel) {
      std::cout << msg;
      opened_ = true;
    }

    return *this;
  }

 private:
  /**
   * @brief Get Label
   *
   * @param level Log Level
   *
   * @return string
   */
  inline std::string GetLabel(const LogLevel level) const;

  bool opened_;
  const LogLevel kMsgLevel;
  static LogLevel kCurrentLogLevel;
};

inline std::string LOG::GetLabel(const LogLevel level) const {
  std::string label;

  switch(level) {
    case LogLevel::INFO: {
      label = "INFO";
      break;
    }
    case LogLevel::WARNING: {
      label = "WARN";
      break;
    }
    case LogLevel::ERROR: {
      label = "ERROR";
      break;
    }
  }

  return label;
}

typedef LOG DLOG;
#define VLOG(x) LOG(INFO)
#define CHECK_NOTNULL(x) assert(0 != x)

#endif  // NO_GLOG

template <typename T>
void IGNORE(T &&) {}

#endif  // COMMONG_LOGGER_H
