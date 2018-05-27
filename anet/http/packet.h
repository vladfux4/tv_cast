#ifndef HTTP_PACKET_H
#define HTTP_PACKET_H

#include <stdint.h>
#include <limits>
#include <vector>
#include <string>
#include <map>

#include <boost/move/unique_ptr.hpp>

#include "anet/net/session.h"
#include "anet/extra/http_parser.h"

namespace anet {
namespace http {

/**
 * @brief The HTTP Packet class
 */
class Packet {
 public:
  /**
   * @brief HTTP Status enum
   */
  enum class Status {
    INIT = -1,
    #define XX(num, name, str) name = num,
    HTTP_STATUS_MAP(XX)
    #undef XX
  };

  /**
   * @brief HTTP Method enum
   */
  enum class Method {
    INIT = -1,
    #define XX(num, name, str) name = num,
    HTTP_METHOD_MAP(XX)
    #undef XX
  };

  /**
   * @brief The HTTP Type enum
   */
  enum class Type {
    INIT,
    REQUEST,
    RESPONSE
  };

  /**
   * @brief HTTP Body type
   */
  typedef std::vector<uint8_t> Buffer;
  typedef boost::movelib::unique_ptr<Buffer> BufferPtr;
  static const uint32_t MAX_BODY_LENGTH
      = std::numeric_limits<uint16_t>::max();

  /**
   * @brief The HTTP Observer class
   */
  class Observer {
   public:
    /**
     * @brief Handle HTTP Packet
     *
     * @param session Server session
     * @param packet Packet
     */
    virtual void HandlePacket(net::SessionPtr session,
                              const Packet& packet) = 0;
    /**
     * @brief Handle Write operation is complete
     *
     * @param session Pointer on session
     */
    virtual void HandleWriteComplete(net::SessionPtr session) = 0;

    /**
     * @brief Handle Close event
     *
     * @param session Pointer on session
     */
    virtual void HandleClose(net::SessionPtr session) = 0;
  };

  /**
   * @brief Constructor
   */
  Packet();

  /**
   * @brief Destructor
   */
  virtual ~Packet();

  /**
   * @brief Init
   *
   * @param http_major HTTP major version
   * @param http_minor HTTP minor version
   * @param status Status code
   * @param method Method
   * @param type Package type
   */
  void Init(const uint16_t http_major, const uint16_t http_minor,
            const Status status, const Method method,
            const Type type);

  /**
   * @brief Set Url
   *
   * @param url URL string
   */
  void SetUrl(const std::string& url);

  /**
   * @brief Add new header field
   *
   * @param key Field name
   * @param value Field data
   */
  void AddHeaderField(const std::string& key,
                      const std::string& value);

  /**
   * @brief Remove header field by name
   *
   * @param key Field name
   */
  void RemoveHeaderField(const std::string& key);

  /**
   * @brief Assign body buffer to packet
   *
   * @param body Pointer on buffer
   */
  void AssignBody(BufferPtr body);

  /**
   * @brief Serialize packet
   *
   * @return buffer
   */
  net::Session::BufferPtr Serialize() const;

  /**
   * @brief Get Status
   *
   * @return HTTP status
   */
  inline const Status GetStatus() const;

  /**
   * @brief Get method
   *
   * @return HTTP Method
   */
  inline const Method GetMethod() const;

  /**
   * @brief Get type
   *
   * @return HTTP Packet type
   */
  inline const Type GetType() const;

  /**
   * @brief Get Url
   *
   * @return string
   */
  inline const std::string& GetUrl() const;

  private:
  /**
   * @brief Get HTTP Status name
   *
   * @param status HTTP Status
   *
   * @return string
   */
  const char* GetStatusName(http_status status) const;

  uint16_t http_major_;
  uint16_t http_minor_;
  Status status_;
  Method method_;
  Type type_;
  std::string url_;
  std::map<std::string, std::string> header_fields_;
  BufferPtr body_;
};

const Packet::Status Packet::GetStatus() const {
  return status_;
}

const Packet::Method Packet::GetMethod() const {
  return method_;
}

const Packet::Type Packet::GetType() const {
  return type_;
}

inline const std::string& Packet::GetUrl() const {
  return url_;
}

}  // namespace http
}  // namespace anet

#endif  //HTTP_PACKET_H
