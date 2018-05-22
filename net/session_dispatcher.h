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
   * @brief Session pointer
   */
  typedef boost::shared_ptr<Session> SessionPtr;

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

  /**
   * @brief Handle packet
   *
   * @param session Reference on session
   * @param buffer Buffer
   *
   * @return handle status
   */
  virtual Status Handle(net::Session& session,
      const boost::asio::const_buffer& buffer) override;

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
