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

void Packet::AddHeaderField(const std::string& key,
                            const std::string& value) {
  header_fields_[key] = value;
}

void Packet::RemoveHeaderField(const std::string& key) {
  header_fields_.erase(key);
}

void Packet::AssignBody(Packet::BodyPtr body) {
  body_ = boost::move(body);
}

}  // namespace http
