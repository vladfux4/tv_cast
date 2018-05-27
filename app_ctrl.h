#ifndef APP_CTRL_H
#define APP_CTRL_H

#include <boost/property_tree/ptree.hpp>

#include "anet/net/packet_observer.h"
#include "anet/http/packet.h"
#include "anet/http/url.h"
#include "anet/net/session.h"

#include <stdint.h>

class CommunicationController;

struct DeviceNotification {
  enum class Type {
    INIT,
    DISCOVER,
    ALIVE
  };

  DeviceNotification()
      : type(Type::INIT),
        ttl(0),
        remote(),
        sid(),
        v1_url(),
        v2_url() {}

  Type type;
  uint32_t ttl;
  std::string remote;
  std::string sid;
  anet::http::Url v1_url;
  anet::http::Url v2_url;

  static Type ConvertToType(const std::string& str);
};

struct DeviceInfo {
  DeviceInfo()
      : stopped(),
        paused(),
        buffering(),
        position(),
        duration(),
        timestamp(),
        platform(),
        year(),
        version(),
        firmware(),
        model_id(),
        real_model() {}

  bool stopped;
  bool paused;
  bool buffering;
  uint32_t position;
  uint32_t duration;
  uint32_t timestamp;
  std::string platform;
  std::string year;
  std::string version;
  std::string firmware;
  std::string model_id;
  std::string real_model;
};

class ApplicationController {
 public:
  /**
   * @brief Constructor
   *
   * @param com_ctrl Reference on communication controller
   */
  ApplicationController(CommunicationController& com_ctrl);

  /**
   * @brief Destructor
   */
  virtual ~ApplicationController();

  /**
   * @brief Handle Device broadcast notification
   *
   * @param buffer JSON data
   */
  virtual void HandleUdpData(const boost::asio::const_buffer& buffer);
  virtual void NotifyUdpDataSend();

  /**
   * @brief Handle HTTP Service package
   *
   * @param session TCP Session
   * @param packet HTTP Packet
   */
  virtual void HandleServicePacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet);
  virtual void NotifyServiceDataSend(anet::net::SessionPtr session);
  virtual void NotifyServiceSessionClose(anet::net::SessionPtr session);

  /**
   * @brief Handle HTTP Communication loop package
   *
   * @param session TCP Session
   * @param packet HTTP Packet
   */
  virtual void HandleLoopPacket(anet::net::SessionPtr session,
                   const anet::http::Packet& packet);
  virtual void NotifyLoopDataSend(anet::net::SessionPtr session);
  virtual void NotifyLoopSessionClose(anet::net::SessionPtr session);

 private:
  /**
   * @brief Send Get config service HTTP request
   */
  void SendGetConfigRequest();

  /**
   * @brief Handle VideoList HTTP GET Request
   *
   * @param url URL
   */
  void HandleVideoListRequest(const anet::http::Url& url);

  /**
   * @brief Response on VideoList request
   */
  void ResponseVideoList();

  /**
   * @brief Parse DeviceNotification JSON tree
   *
   * @param tree Reference on JSON tree
   *
   * @return result
   */
  bool ParseDeviceNotification(boost::property_tree::ptree& tree);

  /**
   * @brief Update device info
   *
   * @param value Refrence on value to update
   * @param key Key in url query
   * @param url Reference on URL
   */
  void UpdateDeviceInfo(bool& value, const std::string& key,
                        const anet::http::Url& url);
  void UpdateDeviceInfo(uint32_t& value, const std::string& key,
                        const anet::http::Url& url);
  void UpdateDeviceInfo(std::string& value, const std::string& key,
                        const anet::http::Url& url);

  /**
   * @brief Convert string to to bool
   *
   * @param str String
   *
   * @return bool
   */
  inline bool ToBool(const std::string& str) const;

  CommunicationController& com_ctrl_;
  DeviceNotification notification_;
  bool notification_received_;
  DeviceInfo device_info_;
  anet::net::SessionPtr loop_session_;
};

inline bool ApplicationController::ToBool(const std::string& str) const {
  return ("true" == str);
}

#endif  // APP_CTRL_H
