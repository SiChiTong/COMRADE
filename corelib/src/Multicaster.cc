#include "Multicaster.hh"
#include "Configurator.hh"
#include "Logger.hh"
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <ctime>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <glibmm/main.h>

using namespace std;
using namespace Comrade::Corelib;

#define MAXSOCKADDR 128

Multicaster::Multicaster()
{
	log=Logger::get_singleton().get_category_ptr("Multicaster");
	Configurator &c=Configurator::get_singleton();
	
	*log << logDEBUG << "Multicaster starting up" << logENDL;
	
	group=c.get_network_group();
	port=c.get_network_port();
	device=c.get_network_device();
	//device="eth0";
	rxbuffersize=c.get_network_rxbuffersize();
	rxbuffer=new unsigned char[rxbuffersize];
	*log << logINFO << "Using receive buffer size of " << rxbuffersize << logENDL;
	*log << logINFO << "using interface " << device << logENDL;
	//char *ifname="eth0"; // TODO: read from config file
	
	const int on = 1;
	socketfd = udp_client(group.c_str(), port.c_str(), (void**)&address, &malen);
	if(socketfd < 0)
		*log << logERROR << "socket error" << logENDL;
	if((setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
		*log << logERROR << "setsockopt SO_REUSEADDR error" << logENDL;
	if((setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, &rxbuffersize, sizeof(rxbuffersize))) < 0)
		*log << logERROR << "setsockopt SO_RCVBUF error" << logENDL;
	if((mcast_set_if(socketfd, device.c_str(), 0)) < 0)
		*log << logERROR << "mcast_set_if error" << logENDL;
	if((bind(socketfd, address, malen)) < 0)
		*log << logERROR << "bind error" << logENDL;	
	if((mcast_join(socketfd, address, malen, device.c_str(), 0)) < 0)
		*log << logERROR << "mcast_join error" << logENDL;
	*log << logINFO << "Successfully joined multicast group " << group << logENDL;
	Glib::signal_io().connect(sigc::mem_fun(*this,&Multicaster::receiver), socketfd, Glib::IO_IN);
}

Multicaster::~Multicaster()
{
	mcast_leave(socketfd, address, malen);
	delete[] rxbuffer;
}

int Multicaster::transmit(const unsigned char* data, int datasize)
{
	return sendto(socketfd, (const void*)data, datasize, 0, address, malen);
}

bool Multicaster::receiver(Glib::IOCondition condition)
{
	sockaddr* fromaddr;
	socklen_t len;
	int n = read(socketfd, &rxbuffer[0], rxbuffersize);
	//int n=recvfrom(socketfd, (void *)rxbuffer, rxbuffersize, 0, fromaddr, &len);
	//cout << "Received " << n << " bytes" << endl;
	on_data_receive(rxbuffer,n);
	return true;
}

int Multicaster::mcast_get_if(int sockfd)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			/* TODO: similar to mcast_set_if() */
			return(-1);
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			u_int index;
			socklen_t len;

			len = sizeof(index);
			if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index, &len) < 0)
				return(-1);
			return(index);
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::mcast_get_loop(int sockfd)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			u_char flag;
			socklen_t len;

			len = sizeof(flag);
			if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, &len) < 0)
				return(-1);
			return(flag);
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			u_int flag;
			socklen_t len;

			len = sizeof(flag);
			if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, &len) < 0)
				return(-1);
			return(flag);
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::mcast_get_ttl(int sockfd)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			u_char ttl;
			socklen_t len;

			len = sizeof(ttl);
			if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, &len) < 0)
				return(-1);
			return(ttl);
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			int hop;
			socklen_t len;

			len = sizeof(hop);
			if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hop, &len) < 0)
				return(-1);
			return(hop);
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::mcast_join(int sockfd, const sockaddr *sa, socklen_t salen, const char *ifname, u_int ifindex)
{
	switch (sa->sa_family)
	{
		case AF_INET:
		{
			ip_mreq mreq;
			ifreq ifreq;

			memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) sa)->sin_addr, sizeof(struct in_addr));

			if (ifindex > 0)
			{
				if (if_indextoname(ifindex, ifreq.ifr_name) == NULL)
				{
					errno = ENXIO;	/* i/f index not found */
					return(-1);
				}
				goto doioctl;
			}
			else if (ifname != NULL)
			{
				strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
				if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					return(-1);
				memcpy(&mreq.imr_interface, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
			}
			else
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

			
			int ret=(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)));
			localip=inet_ntoa(mreq.imr_interface);
			
			(*log) << logINFO << "Our IP address appears to be: " << localip << logENDL;
			return ret;
		}
		/* end mcast_join1 */

		/* include mcast_join2 */
		#ifdef	IPV6
		case AF_INET6:
		{
			ipv6_mreq mreq6;

			memcpy(&mreq6.ipv6mr_multiaddr, &((struct sockaddr_in6 *) sa)->sin6_addr, sizeof(struct in6_addr));

			if (ifindex > 0)
				mreq6.ipv6mr_interface = ifindex;
			else if (ifname != NULL)
				if ( (mreq6.ipv6mr_interface = if_nametoindex(ifname)) == 0)
				{
					errno = ENXIO;	/* i/f name not found */
					return(-1);
				}
			else
				mreq6.ipv6mr_interface = 0;

			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(mreq6)));
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}
/* end mcast_join2 */

int Multicaster::mcast_leave(int sockfd, const sockaddr *sa, socklen_t salen)
{
	switch (sa->sa_family)
	{
		case AF_INET:
		{
			ip_mreq mreq;

			memcpy(&mreq.imr_multiaddr, &((struct sockaddr_in *) sa)->sin_addr, sizeof(struct in_addr));
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);
			return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)));
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			ipv6_mreq mreq6;

			memcpy(&mreq6.ipv6mr_multiaddr, &((struct sockaddr_in6 *) sa)->sin6_addr, sizeof(struct in6_addr));
			mreq6.ipv6mr_interface = 0;
			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &mreq6, sizeof(mreq6)));
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::mcast_set_if(int sockfd, const char *ifname, u_int ifindex)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			in_addr inaddr;
			ifreq ifreq;

			if (ifindex > 0)
			{
				if (if_indextoname(ifindex, ifreq.ifr_name) == NULL)
				{
					errno = ENXIO;	/* i/f index not found */
					return(-1);
				}
				goto doioctl;
			}
			else if (ifname != NULL)
			{
				strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
doioctl:
				if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					return(-1);
				memcpy(&inaddr, &((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
			}
			else
				inaddr.s_addr = htonl(INADDR_ANY);	/* remove prev. set default */

			return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(struct in_addr)));
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			u_int index;

			if ( (index = ifindex) == 0)
			{
				if (ifname == NULL)
				{
					errno = EINVAL;	/* must supply either index or name */
					return(-1);
				}
				if ( (index = if_nametoindex(ifname)) == 0)
				{
					errno = ENXIO;	/* i/f name not found */
					return(-1);
				}
			}
			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index, sizeof(index)));
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::mcast_set_loop(int sockfd, int onoff)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			u_char flag;

			flag = onoff;
			return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag)));
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			u_int flag;

			flag = onoff;
			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &flag, sizeof(flag)));
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}
/* end mcast_set_loop */

int Multicaster::sockfd_to_family(int sockfd)
{
	union
	{
		sockaddr sa;
		char data[MAXSOCKADDR];
	} un;
	socklen_t len;

	len = MAXSOCKADDR;
	if (getsockname(sockfd, (sockaddr *) un.data, &len) < 0)
		return(-1);
	return(un.sa.sa_family);
}
/* end sockfd_to_family */

int Multicaster::mcast_set_ttl(int sockfd, int val)
{
	switch (sockfd_to_family(sockfd))
	{
		case AF_INET:
		{
			u_char ttl;

			ttl = val;
			return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)));
		}

		#ifdef	IPV6
		case AF_INET6:
		{
			int hop;

			hop = val;
			return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hop, sizeof(hop)));
		}
		#endif

		default:
			errno = EPROTONOSUPPORT;
			return(-1);
	}
}

int Multicaster::udp_client(const char *host, const char *serv, void **saptr, socklen_t *lenp)
{
	int sockfd, n;
	addrinfo hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
		*log << logFATAL << "udp_client error for " << host << ", "
			<< serv << ": " << gai_strerror(n) << logENDL;
		exit(1);
	}
	ressave = res;

	do
	{
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break;		/* success */
	}
	while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final socket() */
	{
		*log << logFATAL << "udp_client error for " << host << ", " << serv << logENDL;
		exit(1);
	}

	*saptr = malloc(res->ai_addrlen);
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_client */
