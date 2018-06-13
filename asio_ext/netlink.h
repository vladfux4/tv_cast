#ifndef ASIO_EXT_NETLINK_H
#define ASIO_EXT_NETLINK_H

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/basic_raw_socket.hpp>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include "asio_ext/netlink_endpoint.h"

namespace asio_ext {

class netlink {
 public:
  /// The type of a UDP endpoint.
  typedef asio_ext::netlink_endpoint<netlink> endpoint;
  typedef boost::asio::basic_raw_socket<netlink> socket;

  /// Construct to represent the Netlink ROUTE protocol.
  static netlink route() {
    return netlink(NETLINK_ROUTE);
  }

  /// Default constructor
  netlink() : protocol_(0) {}

  /// Obtain an identifier for the type of the protocol.
  int type() const {
    return BOOST_ASIO_OS_DEF(SOCK_RAW);
  }

  /// Obtain an identifier for the protocol.
  int protocol() const {
    return protocol_;
  }

  /// Obtain an identifier for the protocol family.
  int family() const {
    return PF_NETLINK;
  }

  /// Compare two protocols for equality.
  friend bool operator==(const netlink& p1, const netlink& p2) {
    return (p1.protocol_ == p2.protocol_);
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const netlink& p1, const netlink& p2) {
    return (p1.protocol_ != p2.protocol_);
  }

 private:

  // Construct with a specific protocol.
  explicit netlink(const int& protocol)
    : protocol_(protocol) {
  }

  int protocol_;
};

} // namespace asio_ext

#endif // ASIO_EXT_NETLINK_H
