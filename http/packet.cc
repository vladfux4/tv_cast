#include <sstream>

#include "http/packet.h"

#include "common/logger.h"

namespace http {

Packet::Packet()
    : http_major_(0), http_minor_(0),
      status_(Packet::Status::INIT), method_(Packet::Method::INIT),
      type_(Type::INIT),
      header_fields_(),
      body_(nullptr) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

Packet::~Packet() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

void Packet::Init(const uint16_t http_major, const uint16_t http_minor,
    const Packet::Status status, const Packet::Method method,
    const Type type) {
  http_major_ = http_major;
  http_minor_ = http_minor;
  status_ = status;
  method_ = method;
  type_ = type;
}

void Packet::SetUrl(const std::string& url) {
  url_ = url;
}

void Packet::AddHeaderField(const std::string& key,
                            const std::string& value) {
  header_fields_[key] = value;
}

void Packet::RemoveHeaderField(const std::string& key) {
  header_fields_.erase(key);
}

void Packet::AssignBody(Packet::BufferPtr body) {
  body_ = boost::move(body);
}

net::Session::BufferPtr Packet::Serialize() const {
  net::Session::BufferPtr buffer(nullptr);
  std::string endl = "\r\n";

  if (Type::INIT != type_) {
    std::string version =
        "HTTP/" + std::to_string(http_major_)
        + "." + std::to_string(http_minor_);

    std::string start_string;
    if (Type::REQUEST == type_) {
      start_string =
          std::string(http_method_str(static_cast<http_method>(method_)))
          + " " + url_ + " " + version;
    } else if (Type::RESPONSE == type_) {
      start_string = version
          + " " + std::to_string(static_cast<int>(status_))
          + " " + GetStatusName(static_cast<http_status>(status_));
    }
    start_string += endl;

    std::stringstream header_stream;
    for (auto it = header_fields_.begin(); it != header_fields_.end(); it++) {
      header_stream << it->first << ": " << it->second << endl;
    }
    header_stream << endl;
    std::string headers = header_stream.str();

    uint32_t size = start_string.size() + headers.size();
    if (nullptr != body_) {
      size += body_->size();
    }

    buffer = net::Session::BufferPtr(new net::Session::Buffer(size));
    uint32_t i = 0;
    memcpy(&buffer->at(0), &start_string.at(0), start_string.size());
    i += start_string.size();
    memcpy(&buffer->at(i), &headers.at(0), headers.size());
    i += headers.size();
    if (nullptr != body_) {
      memcpy(&buffer->at(i), &body_->at(0), body_->size());
    }
  }

  return buffer;
}

const char* Packet::GetStatusName(http_status status) const {
  const char* str = "INVALID_STATUS";

#define XX(num, name, string) case HTTP_STATUS_##name: { str=#name; break; }
  switch (status) {
    HTTP_STATUS_MAP(XX)
  }
#undef XX

  return str;
}

}  // namespace http
