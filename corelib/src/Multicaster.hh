#ifndef COMRADE_CORELIB_MULTICASTER
#define COMRADE_CORELIB_MULTICASTER

#include <sigc++/sigc++.h>
#include <sys/socket.h>
#include <glibmm/main.h>
#include <netinet/in.h>
#include <string>

#include "Logger.hh"

namespace Comrade
{

namespace Corelib
{
	
class Multicaster: public sigc::trackable, public Singleton<Multicaster>
{
	public:
		Multicaster();
		~Multicaster();
		int transmit(const unsigned char* data, int datasize);
		sigc::signal<void,unsigned char*, int> on_data_receive;
	private:
		std::string group;
		std::string port;
		std::string device;
		int socketfd;
		sockaddr* address;
		socklen_t malen;	

		int mcast_get_if(int sockfd);
		int mcast_get_loop(int sockfd);
		int mcast_get_ttl(int sockfd);
		int mcast_join(int sockfd, const sockaddr *sa, socklen_t salen, const char *ifname, u_int ifindex);
		int mcast_leave(int sockfd, const sockaddr *sa, socklen_t salen);
		int mcast_set_if(int sockfd, const char *ifname, u_int ifindex);
		int mcast_set_loop(int sockfd, int onoff);
		int mcast_set_ttl(int sockfd, int val);
		int udp_client(const char *host, const char *serv, void **saptr, socklen_t *lenp);
		int sockfd_to_family(int sockfd);	
	
		bool receiver(Glib::IOCondition);
		log4cpp::Category *log;
		unsigned char *rxbuffer;
		int rxbuffersize;
		std::string localip;
	
};
	
} // namespace Corelib

} // namespace Comrade

#endif //COMRADE_CORELIB_MULTICASTER
