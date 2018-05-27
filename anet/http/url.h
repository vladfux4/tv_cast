#ifndef HTTP_URL_H
#define HTTP_URL_H

#include <string>
#include <map>

#include "anet/extra/http_parser.h"

namespace anet {
namespace http {

class Url {
 public:
  typedef std::map<std::string, std::string> QueryParameters;

  /**
   * @brief Constructor
   */
  Url();

  /**
   * @brief Constructor
   *
   * @param str URL string
   */
  Url(const std::string& str);

  /**
   * @brief Destructor
   */
  ~Url();

  /**
   * @brief Get query parameter by key
   *
   * @param key Key
   *
   * @return value
   */
  const std::string& GetQueryParameter(const std::string& key) const;

  /**
   * @brief Get query parametres map
   *
   * @return Reference on query map
   */
  inline const QueryParameters& GetQueryMap() const;

  /**
   * @brief Parse
   *
   * @param[in] str Source
   * @param[out] url Reference on url
   */
  static void Parse(const std::string& str, Url& url);

  std::string schema;
  std::string host;
  uint16_t port;
  std::string path;
  std::string query;
  std::string fragment;
  std::string user_info;
 private:

  /**
   * @brief Check is bit set
   *
   * @param field_set Set
   * @param field http_parser field
   *
   * @return result
   */
  inline static bool IsSet(const uint16_t field_set,
                              const http_parser_url_fields field);

  /**
   * @brief Get string from data buffer
   *
   * @param parser Pointer on http_parser_url
   * @param buffer Data buffer
   * @param field http_parser field
   *
   * @return string
   */
  static std::string GetString(const http_parser_url* parser,
                               const char* buffer,
                               const http_parser_url_fields field);

  /**
   * @brief Parse URL query
   *
   * @param str Query string
   */
  void ParseQuery(const std::string& str);

  QueryParameters query_parameters_;
};

inline const Url::QueryParameters& Url::GetQueryMap() const {
  return query_parameters_;
}

inline bool Url::IsSet(const uint16_t field_set,
                   const http_parser_url_fields field) {
  return (0 != (field_set & (1 << field)));
}

}  // namespace http
}  // namespace anet

#endif  //HTTP_URL_H
