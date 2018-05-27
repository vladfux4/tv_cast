#include "anet/http/url.h"
#include "common/logger.h"

namespace anet {
namespace http {

Url::Url()
    : schema(),
      host(),
      port(0),
      path(),
      query(),
      fragment(),
      user_info() {
}

Url::Url(const std::string& str)
    : schema(),
      host(),
      port(0),
      path(),
      query(),
      fragment(),
      user_info() {
  Parse(str, *this);
}

Url::~Url() {
}

const std::string& Url::GetQueryParameter(const std::string& key) const {
  return query_parameters_.at(key);
}

void Url::Parse(const std::string& str, Url& url) {
  http_parser_url parser;
  http_parser_url_init(&parser);
  const char* data = &str.at(0);
  uint16_t length = str.size();

  int result = http_parser_parse_url(data, length, false, &parser);
  if (0 == result) {
    url.schema = GetString(&parser, data, UF_SCHEMA);
    url.host = GetString(&parser, data, UF_HOST);
    url.port = parser.port;
    url.path = GetString(&parser, data, UF_PATH);
    url.query = GetString(&parser, data, UF_QUERY);
    url.fragment = GetString(&parser, data, UF_FRAGMENT);
    url.user_info = GetString(&parser, data, UF_USERINFO);

    url.ParseQuery(url.query);
  }
}

std::string Url::GetString(const http_parser_url* parser,
                           const char* buffer,
                           const http_parser_url_fields field) {
  std::string str;

  if (IsSet(parser->field_set, field)) {
    const char* data = (buffer + parser->field_data[field].off);
    const uint16_t length = parser->field_data[field].len;
    str = std::string(data, length);
  }

  return str;
}

void Url::ParseQuery(const std::string& str) {
  auto key_begin = str.begin();
  auto key_end = str.begin();
  auto value_begin = str.begin();
  auto value_end = str.begin();

  for (auto it = str.begin(); it != str.end(); ++it) {
    if ('=' == *it) {
      key_end = (it);

      if ((it + 1) != str.end()) {
        value_begin = (it + 1);
        value_end = value_begin;
      }
    }

    if ('&' == *it) {
      value_end = (it);

      if ((key_begin != key_end) && (value_begin != value_end)) {
        query_parameters_[std::string(key_begin, key_end)] =
            std::string(value_begin, value_end);
      }

      if ((it + 1) != str.end()) {
        key_begin = (it + 1);
        key_end = key_begin;
      }
    }
  }

  value_end = str.end();

  if ((key_begin != key_end) && (value_begin != value_end)) {
    query_parameters_[std::string(key_begin, key_end)] =
        std::string(value_begin, value_end);
  }
}

}  // namespace http
}  // namespace anet
