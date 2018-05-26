#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "anet/net/packet_observer.h"

namespace anet {
namespace udp {

class Server {
 public:
  /**
   * @brief Byte buffer type
   */
  typedef std::vector<uint8_t> Buffer;
  typedef boost::shared_ptr<Buffer> BufferPtr;

  /**
   * @brief Constructor
   *
   * @param io_service ASIO IO Service
   * @param port TCP connection local port
   */
  Server(boost::asio::io_service& io_service,
         const boost::asio::ip::udp::endpoint& listen_endpoint);

  /**
   * @brief Destructor
   */
  virtual ~Server();

  /**
   * @brief Register observer
   *
   * @param observer Reference on observer
   */
  void RegisterObserver(net::PacketObserver& observer);

  /**
   * @brief Join to multicast group
   *
   * @param multicast_address Address
   */
  void JoinMulticastGroup(const boost::asio::ip::address& multicast_address);

  /**
   * @brief Connect to target
   *
   * @param target Target
   */
  void Connect(const boost::asio::ip::udp::endpoint& target);

  /**
   * @brief Start
   */
  void Start();

  /**
   * @brief Receive data
   */
  void Receive();

  /**
   * @brief Send data
   *
   * @param buffer Data buffer
   */
  void Send(BufferPtr buffer);

  /**
   * @brief Handle receive from socket operation completed
   *
   * @param error Error code
   * @param bytes_transferred Data length
   */
  void HandleReceive(const boost::system::error_code& error, size_t bytes_transferred);

  /**
   * @brief Handle send to socket operation completed
   *
   * @param error Error code
   */
  void HandleSend(const boost::system::error_code& error, net::Session::BufferPtr buffer);

  private:
  boost::asio::io_service& io_service_;
  net::PacketObserver* observer_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  enum { MAX_BUFFER_SIZE = 2048 };
  boost::array<uint8_t, MAX_BUFFER_SIZE> buffer_;
};

} // namespace udp
} // namespace anet

#endif  // UDP_SERVER_H
