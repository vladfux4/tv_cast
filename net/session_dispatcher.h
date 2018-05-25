#ifndef NET_SESSION_DISPATCHER
#define NET_SESSION_DISPATCHER

#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/unique_ptr.hpp>

#include "net/packet_handler.h"
#include "net/session.h"
#include "net/session_accessor.h"

namespace net {

class SessionDispatcher {
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
   * @brief Register packet handler creator
   *
   * @param creator Reference on creator
   */
  void RegisterCreator(net::PacketHandlerCreator& creator);

  /**
   * @brief Close Session
   *
   * @param accessor Session's accessor
   */
  void CloseSession(const SessionAccessor& accessor);

 protected:
  /**
   * @brief SessionAccessor unique pointer type
   */
  typedef boost::movelib::unique_ptr<SessionAccessor> SessionAccessorPtr;

  /**
   * @brief Create accessor for new session
   */
  SessionAccessorPtr GetNewSessionAccessor();

  /**
   * @brief Add session to pool
   *
   * @param session Pointer on session
   */
  void AddSession(SessionPtr session);

  net::PacketHandlerCreator* creator_;
 private:
  /**
   * @brief Session pool type
   */
  typedef boost::container::vector<SessionPtr> SessionPool;

  SessionPool session_pool_;
};

} // namespace net

#endif  // NET_SESSION_DISPATCHER
