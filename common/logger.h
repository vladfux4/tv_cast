#ifndef COMMONG_LOGGER_H
#define COMMONG_LOGGER_H

#include <iostream>

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
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
    case LogLevel::DEBUG: {
      label = "DEBUG";
      break;
    }
    case LogLevel::INFO: {
      label = "INFO";
      break;
    }
    case LogLevel::WARN: {
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

template <typename T>
void IGNORE(T &&) {}

#endif  // COMMONG_LOGGER_H
