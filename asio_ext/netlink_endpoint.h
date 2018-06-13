#ifndef ASIO_EXT_NETLINK_ENDPOINT_H
#define ASIO_EXT_NETLINK_ENDPOINT_H

#include <boost/asio/detail/config.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/detail/endpoint.hpp>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

namespace asio_ext {

template <typename Protocol>
class netlink_endpoint {
 public:
  /// The protocol type associated with the endpoint.
  typedef Protocol protocol_type;
  typedef boost::asio::detail::socket_addr_type data_type;

  /// Default constructor.
  netlink_endpoint() {
    sockaddr_.nl_family = PF_NETLINK;
    sockaddr_.nl_groups = 0;
    sockaddr_.nl_pid = getpid();
  }

  netlink_endpoint(const uint32_t& group) {
    sockaddr_.nl_family = PF_NETLINK;
    sockaddr_.nl_groups = group;
    sockaddr_.nl_pid = getpid();
  }

  /// Copy constructor.
  netlink_endpoint(const netlink_endpoint& other) {
    sockaddr_ = other.sockaddr_;
  }

  /// Assign from another endpoint.
  netlink_endpoint& operator=(const netlink_endpoint& other) {
    sockaddr_ = other.sockaddr_;
    return *this;
  }

  /// The protocol associated with the endpoint.
  protocol_type protocol() const {
    return protocol_type();
  }

  /// Get the underlying endpoint in the native type.
  data_type* data() {
    return reinterpret_cast<data_type*>(&sockaddr_);
  }

  /// Get the underlying endpoint in the native type.
  const data_type* data() const {
    return reinterpret_cast<const data_type*>(&sockaddr_);
  }

  /// Get the underlying size of the endpoint in the native type.
  std::size_t size() const {
    return sizeof(sockaddr_);
  }

  /// Set the underlying size of the endpoint in the native type.
  void resize(std::size_t new_size) {
  }

  /// Get the capacity of the endpoint in the native type.
  std::size_t capacity() const {
    return sizeof(sockaddr_);
  }

  /// Compare two endpoints for equality.
  friend bool operator==(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return (e1.sockaddr_.nl_groups == e2.sockaddr_.nl_groups);
  }

  /// Compare two endpoints for inequality.
  friend bool operator!=(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return !(e1 == e2);
  }

  /// Compare endpoints for ordering.
  friend bool operator<(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return (e1.sockaddr_.nl_groups < e2.sockaddr_.nl_groups);
  }

  /// Compare endpoints for ordering.
  friend bool operator>(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return (e1.sockaddr_.nl_groups > e2.sockaddr_.nl_groups);
  }

  /// Compare endpoints for ordering.
  friend bool operator<=(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return !(e2 < e1);
  }

  /// Compare endpoints for ordering.
  friend bool operator>=(const netlink_endpoint<Protocol>& e1,
      const netlink_endpoint<Protocol>& e2) {
    return !(e1 < e2);
  }

 private:
  // The underlying IP endpoint.
  sockaddr_nl sockaddr_;
};

} // namespace asio_ext

#endif // ASIO_EXT_NETLINK_ENDPOINT_H
