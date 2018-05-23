#ifndef NET_SESSION_DISPATCHER
#define NET_SESSION_DISPATCHER

#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>

#include "net/packet_handler.h"
#include "net/session.h"
#include "net/session_accessor.h"

namespace net {

class SessionDispatcher : public net::PacketHandler {
 public:
  /**
   * @brief Constructor
   */
  SessionDispatcher();

  /**
   * @brief Destructor
   */
  virtual ~SessionDispatcher();

  /**
   * @brief Register packets handler
   *
   * @param packet_handler Reference on handler
   */
  void RegisterHandler(net::PacketHandler& handler);

  virtual Status Handle(net::SessionPtr session,
      const boost::asio::const_buffer& buffer) override;

  virtual void HandleClose(net::SessionPtr session) override;

  /**
   * @brief Close Session
   *
   * @param accessor Session's accessor
   */
  void CloseSession(const SessionAccessor& accessor);

 protected:
  /**
   * @brief Create accessor for new session
   *
   * @return accessor
   */
  const SessionAccessor GetNewSessionAccessor();

  /**
   * @brief Add session to pool
   *
   * @param session Pointer on session
   */
  void AddSession(SessionPtr session);

 private:
  /**
   * @brief Session pool type
   */
  typedef boost::container::vector<SessionPtr> SessionPool;

  net::PacketHandler* packet_handler_;
  SessionPool session_pool_;
};

} // namespace net

#endif  // NET_SESSION_DISPATCHER
