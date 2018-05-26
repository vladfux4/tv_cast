#ifndef NET_SESSION_DISPATCHER
#define NET_SESSION_DISPATCHER

#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/unique_ptr.hpp>

#include "anet/net/session_observer.h"
#include "anet/net/session.h"
#include "anet/net/session_accessor.h"

namespace anet {
namespace net {

class SessionHandler {
 public:
  /**
   * @brief Constructor
   */
  SessionHandler();

  /**
   * @brief Destructor
   */
  virtual ~SessionHandler();

  /**
   * @brief Register packet handler creator
   *
   * @param creator Reference on creator
   */
  void RegisterCreator(net::SessionObserverCreator& creator);

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

  net::SessionObserverCreator* creator_;
 private:
  /**
   * @brief Session pool type
   */
  typedef boost::container::vector<SessionPtr> SessionPool;

  SessionPool session_pool_;
};

} // namespace net
} // namespace anet

#endif  // NET_SESSION_DISPATCHER
