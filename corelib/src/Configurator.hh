//
// Configurator.hh
//

#ifndef COMRADE_CORELIB_CONFIGURATOR
#define COMRADE_CORELIB_CONFIGURATOR

#include <map>
#include <vector>
#include <string>
#include <glibmm/markup.h>

#include "Singleton.hh"

using namespace std;
using namespace Glib::Markup;

namespace Comrade
{

namespace Corelib
{

/** \brief Data structure that holds the information of an XML node and pointers to subnodes. 
 */
class XMLTree
{
	public:
		XMLTree();
		XMLTree(const string& lname, const Parser::AttributeMap &lattributes);
		~XMLTree();
		
		void 			add_subnode(XMLTree* node);
		void 			set_text(const string& ltext);
		const string& 		get_text() const;
		XMLTree* 		get_subnode(const string &nodename);
		pair<multimap<string,XMLTree *>::iterator,multimap<string,XMLTree *>::iterator> get_subnode_list(const string& nodename);
		friend ostream& operator<<(ostream &output, XMLTree &xmltree);
	//	XMLTree& 		operator[](const string& nodename);

	private:
		multimap<string,XMLTree*>	subnodes;
		Parser::AttributeMap	attributes;
		string			name;
		string			text;
};

/** \brief The class which loads an XML configuration filename
 *
 *  This class loads an XML configuration file, and retrieves all the required
 *  configuration data from it. If you need to add extra configuration items
 *  add the necessary items to this.
 *
 *  Uses the \c Glib::Markup::Parser XML subset parser from the glibmm library.
 *  \warning Feed it only very simple XML, this supports only a subset of the complete XML spec.
 */
class Configurator: public Glib::Markup::Parser, public Singleton<Configurator>
{
	public:
		Configurator();
		~Configurator();

		void			open_file(const string& filename);
		void			save_file(const string& filename);

		string			get_config_string(string name, string defaultval);
		int			get_config_int(string name, int defaultval);		
		double			get_config_double(string name, double defaultval);
		bool			get_config_bool(string name, bool defaultval);
		void			set_config_string(string name, string defaultval);
		void			set_config_int(string name, int defaultval);		
		void			set_config_double(string name, double defaultval);
		void			set_config_bool(string name, bool defaultval);
	
		string			get_logger_filename();
		string			get_logger_priority();
	
		string			get_network_group();
		string			get_network_port();
		string			get_network_device();
		int			get_network_rxbuffersize();
		int			get_network_txpacketsize();
	
		string			get_serial_device();
		int			get_serial_rxbuffersize();
		int			get_serial_baudrate();
		int			get_serial_retx_timeout();
		int			get_serial_max_framesize();
	
		string			get_mouse_device();
		int			get_mouse_updateinterval();
		bool			get_mouse_exchange_lr();

		int			get_motorspeeds_leftforward();
		int			get_motorspeeds_leftbackward();
		int			get_motorspeeds_rightforward();
		int			get_motorspeeds_rightbackward();
		
		int			get_motorspeeds_minleftforward();
		int			get_motorspeeds_minleftbackward();
		void			set_motorspeeds_minleftforward(int speed);
		void			set_motorspeeds_minleftbackward(int speed);
		int			get_motorspeeds_minrightforward();
		int			get_motorspeeds_minrightbackward();
		void			set_motorspeeds_minrightforward(int speed);
		void			set_motorspeeds_minrightbackward(int speed);
				
		void			set_motorspeeds_leftforward(int speed);
		void			set_motorspeeds_leftbackward(int speed);
		void			set_motorspeeds_rightforward(int speed);
		void			set_motorspeeds_rightbackward(int speed);
		
		bool			get_motorspeeds_leftinvert();
		bool			get_motorspeeds_rightinvert();
		bool			get_mouse_leftinvert();
		bool			get_mouse_rightinvert();
		
		double			get_odometry_wheelbase();
		double			get_odometry_translationconstant();
		void			get_odometry_wheelbase(double wheelbase);
		void			get_odometry_translationconstant(double constant);

		int			get_compass_minvalue();
		int			get_compass_maxvalue();
		void			set_compass_minvalue(int value);
		void			set_compass_maxvalue(int value);
		
	private:
		vector<XMLTree*>	opentrees;
		XMLTree*		maintree;

		virtual void		on_start_element(ParseContext& context, const Glib::ustring& element_name, const Parser::AttributeMap& attributes);
		virtual void		on_end_element(ParseContext& context, const Glib::ustring& element_name);
		virtual void		on_text(ParseContext& context, const Glib::ustring& text);
};

} // namespace Corelib

} // namespace Comrade

#endif
