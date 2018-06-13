#include <gflags/gflags.h>

#include <iostream>

#include "common/logger.h"
#include <boost/asio.hpp>

#include "asio_ext/netlink.h"

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define IFF_LOWER_UP	0x10000

void handle_netlink(struct nlmsghdr *nlm) {
  int len;
  char ifname[IF_NAMESIZE + 1];
  ifinfomsg *ifi;
  rtattr *rta;

  if (nlm->nlmsg_type == RTM_NEWLINK) {
    len = nlm->nlmsg_len - sizeof(*nlm);
    if ((size_t)len < sizeof(*ifi)) {
      errno = EBADMSG;
      return;
    }

    ifi = (ifinfomsg*)NLMSG_DATA(nlm);
    if (ifi->ifi_flags & IFF_LOOPBACK)
      return;

    rta = (rtattr *) ((char *)ifi + NLMSG_ALIGN(sizeof(*ifi)));
    len = NLMSG_PAYLOAD(nlm, sizeof(*ifi));
    *ifname = '\0';

    while (RTA_OK(rta, len)) {
      switch (rta->rta_type) {
        case IFLA_IFNAME:
          strncpy(ifname, (char*)RTA_DATA(rta), sizeof(ifname));
          break;
      }

      rta = RTA_NEXT(rta, len);
    }
  }

  if (nlm->nlmsg_type == RTM_NEWLINK) {
    len = ifi->ifi_change == ~0U ? 1 : 0;
  }

  std::cout << "Interface " << ifname << " changed status, now: ";

  if((ifi->ifi_flags&IFF_LOWER_UP)==IFF_LOWER_UP) {
    std::cout << " Up" << std::endl;
  } else {
    std::cout << " Down" << std::endl;
  }
}

enum { max_length = 1024 };

int main(int argc, char* argv[]) {
  InitLogger(argv[0]);

  try {
    boost::asio::io_service io_service;

    asio_ext::netlink::socket socket(io_service);
    socket.open(asio_ext::netlink::route());
    socket.bind(asio_ext::netlink::endpoint(RTMGRP_LINK));


    char buffer[max_length];
    int bytes;

    while((bytes=socket.receive(boost::asio::buffer(buffer, max_length)))) {
//      for (uint32_t i = 0; i < bytes; ++i) {
//        std::cout << buffer[i];
//      }
//      std::cout << std::endl;

      struct nlmsghdr *nlm;
      for (nlm = (struct nlmsghdr *)buffer;
           NLMSG_OK(nlm, (size_t)bytes);
           nlm = NLMSG_NEXT(nlm, bytes)) {
        handle_netlink(nlm);
      }
    }

    io_service.run();
  } catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }

  return 0;
}
