#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdint.h>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/move/unique_ptr.hpp>

#include "anet/extra/http_parser.h"
#include "anet/http/packet.h"
#include "anet/net/session_handler.h"

namespace anet {
namespace http {

class Parser {
 public:
  /**
   * @brief Packat pointer type
   */
  typedef boost::movelib::unique_ptr<Packet> PacketPtr;

  /**
   * @brief Member function pointer
   */
  typedef int (Parser::*cpp_http_cb) (http_parser*);

  /**
   * @brief Member function pointer
   */
  typedef int (Parser::*cpp_http_data_cb)
              (http_parser*, const char*, size_t);

  /**
   * @brief Contructor
   */
  Parser();

  /**
   * @brief Destructor
   */
  virtual ~Parser();

  /**
   * @brief Parse buffer
   *
   * @param buffer Data buffer
   *
   * @return status
   */
  net::SessionHandler::Status Parse(const boost::asio::const_buffer& buffer);

  /**
   * @brief Get parse status
   *
   * @return status
   */
  inline net::SessionHandler::Status GetStatus() const;

  /**
   * @brief Get Packet
   *
   * @return HTTP Packet
   */
  inline PacketPtr GetPacket();

  /**
   * @brief Static http parser callback
   */
  static int SHandleMsgBegin(http_parser* c_parser);
  static int SHandleUrl(http_parser* c_parser, const char* at, size_t length);
  static int SHandleStatus(http_parser* c_parser, const char* at, size_t length);
  static int SHandleHeaderField(http_parser* c_parser, const char* at, size_t length);
  static int SHandleHeaderValue(http_parser* c_parser, const char* at, size_t length);
  static int SHandleHeaderComplete(http_parser* c_parser);
  static int SHandleBody(http_parser* c_parser, const char* at, size_t length);
  static int SHandleMsgComplete(http_parser* c_parser);
  static int SHandleHeaderChunk(http_parser* c_parser);

 private:
  /**
   * @brief Parser callbacks
   */
  int HandleMsgBegin(http_parser* c_parser);
  int HandleUrl(http_parser* c_parser, const char* at, size_t length);
  int HandleStatus(http_parser* c_parser, const char* at, size_t length);
  int HandleHeaderField(http_parser* c_parser, const char* at, size_t length);
  int HandleHeaderValue(http_parser* c_parser, const char* at, size_t length);
  int HandleHeaderComplete(http_parser* c_parser);
  int HandleBody(http_parser* c_parser, const char* at, size_t length);
  int HandleMsgComplete(http_parser* c_parser);
  int HandleHeaderChunk(http_parser* c_parser);

  /**
   * @brief Get HTTP Parser error string
   *
   * @param error Error code
   *
   * @return string
   */
  const char* GetErrString(unsigned int error);

  /**
   * @brief Get HTTP status string
   *
   * @param status Status code
   *
   * @return string
   */
  const char* GetStatusString(unsigned int status);

  /**
   * @brief Get HTTP method string
   *
   * @param method Method code
   *
   * @return string
   */
  const char* GetMethodString(unsigned int method);

  /**
   * @brief Handle common callback
   *
   * @param func Pointer on member function
   * @param c_parser Pointer on http_parser
   *
   * @return result
   */
  static int SHandle(cpp_http_cb func, http_parser* c_parser);

  /**
   * @brief Handle data common callback
   *
   * @param func Pointer on member function
   * @param c_parser Pointer on http_parser
   * @param at Data start
   * @param length Length
   *
   * @return result
   */
  static int SHandleData(cpp_http_data_cb func,
                         http_parser* c_parser, const char* at, size_t length);

  net::SessionHandler::Status status_;
  PacketPtr packet_;
  http_parser_settings setting_;
  boost::movelib::unique_ptr<http_parser> parser_;
  std::string current_field_;
  Packet::BufferPtr body_;
  uint32_t body_index_;
  uint32_t content_length_;
};

inline net::SessionHandler::Status Parser::GetStatus() const {
  return status_;
}

Parser::PacketPtr Parser::GetPacket() {
  return boost::move(packet_);
}

}  // namespace http
}  // namespace anet

#endif  //HTTP_PARSER_H
