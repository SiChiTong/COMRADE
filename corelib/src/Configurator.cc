#include "Configurator.hh"

#include <string>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glibmm/markup.h>
#include <glibmm/fileutils.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>

using namespace std;
using namespace Glib::Markup;
using namespace Comrade::Corelib;

XMLTree::XMLTree()
{
}

/**
 * @param lname The name of the new node
 * @param lattributes The attributes of the the node
 */
XMLTree::XMLTree(const string& lname, const Parser::AttributeMap& lattributes)
{
	name=lname;
	attributes=lattributes;
}

XMLTree::~XMLTree()
{
	multimap<string,XMLTree*>::iterator iter;
	for (iter=subnodes.begin(); iter!=subnodes.end(); iter++)
	{
		delete iter->second;
	}
}

/** \brief Adds a subnode to this node
 *  @param node The node to add to this node
 */
void XMLTree::add_subnode(XMLTree *node)
{
	pair<string,XMLTree *> entry(node->name,node);
	subnodes.insert(entry);
}

/** \brief Sets the text contained within this node
 *  @param ltext The text to set
 */
void XMLTree::set_text(const string& ltext)
{
	string str=ltext;
	str.erase(0,str.find_first_not_of(" \t\n")); // strip leading whitespace...
	str.erase(str.find_last_not_of(" \t\n")+1); // ...and trailing whitespace
	if (!str.empty()) text=str;
}

/** \brief Gets the text contained within this node
 *  @return The text contained within the node
 */
const string& XMLTree::get_text() const
{
	return text;
}

/** \brief Gets a subnode of this node
 *  @param nodename The name of the subnode to return
 *  @return Pointer to the subnode
 */
XMLTree* XMLTree::get_subnode(const string& nodename)
{
	multimap<string, XMLTree*>::iterator iter=subnodes.find(nodename);
	if (iter!=subnodes.end()) return iter->second;
		else return NULL;
}

/** \brief Gets a subnode of this node
 *  @param nodename The name of the subnode to return
 *  @return The subnode
 */
/*XMLTree& XMLTree::operator[](const string& nodename)
{
	XMLTree *tmp=get_subnode(nodename);
	if (tmp) return (*tmp) else throw 
}*/

/** \brief Gets list of subnodes
 *  
 *  Returns a list of subnodes with the given name, represented as two iterators -
 *  the start and the end. Iterate through them to get all the subnodes.
 *  @param nodename The name of the subnode list to return
 *  @return A pair of multimap iterators referring to the beginning and end of the list
 */
pair<multimap<string,XMLTree *>::iterator,multimap<string,XMLTree *>::iterator> XMLTree::get_subnode_list(const string& nodename)
{
	return subnodes.equal_range(nodename);
}

/** \brief Overloads operator << to output the XMLTree to an ostream */
ostream& Comrade::Corelib::operator<<(ostream &output, XMLTree &xmltree)
{
	static int depth=0;
	bool printtags=true;
	multimap<string,XMLTree*>::iterator iter;
	int i;
	for (i=0;i<depth;i++) output<<"\t";
	if (xmltree.name.empty())
	{
		printtags=false;
	}
	if (printtags)
	{
		output << "<" << xmltree.name << ">";
		if (xmltree.text.empty())
		{
			output << endl;
		}
		else
		{
			output << xmltree.text;
		}
		depth++;
	}
	for (iter=xmltree.subnodes.begin(); iter!=xmltree.subnodes.end(); iter++)
	{
		output << *(iter->second);
	}
	if (printtags)
	{
		depth--;
		if (xmltree.text.empty())
		{
			for (i=0;i<depth;i++) output<<"\t";
		}
		output << "</" << xmltree.name << ">" << endl;
	}
	return output;
}

Configurator::Configurator()
{
	maintree=new XMLTree;
	opentrees.push_back(maintree);
}

Configurator::~Configurator()
{
	delete maintree;
}

/** \brief Open an XML file and parse it
 *  @param filename The name of the file to open
 */
void Configurator::open_file(const string& filename)
{
	ParseContext parsecontext(*this);
	string xmlcontent;
	try
	{
		xmlcontent=Glib::file_get_contents(filename);	
	}
	catch(Glib::FileError& e)
	{
		// TODO
		//cerr << "Error opening file" << endl;
	}
	try
	{
		parsecontext.parse(xmlcontent);
		parsecontext.end_parse();
	}
	catch(Glib::MarkupError& e)
	{
		// TODO
		//cerr << "Error parsing file" << endl;
	}
}

/** \brief Save the current config to an XML file
 *  @param filename The name of the file to save
 */
void Configurator::save_file(const string& filename)
{
	ofstream f;
	try
	{
		f.open(filename.c_str());
	}
	catch (...)
	{
		// TODO
		cerr << "Error blah"<<endl;
	}
	f << *maintree;
	f.close();
}

void Configurator::on_start_element(ParseContext &context, const Glib::ustring& element_name, const Parser::AttributeMap& attributes)
{
	XMLTree *temptree=new XMLTree(element_name,attributes);
	opentrees.push_back(temptree);
}

void Configurator::on_end_element(ParseContext& context, const Glib::ustring& element_name)
{
	int depth=opentrees.size();
	opentrees[depth-2]->add_subnode(opentrees[depth-1]);
	opentrees.pop_back();
}

void Configurator::on_text(ParseContext &context, const Glib::ustring& text)
{
	int depth=opentrees.size();
	opentrees[depth-1]->set_text(text);
}

//virtual void on_error(ParseContext& context, const Glib::MarkupError& error)
//{
	// TODO
//}

/** \brief Retrieve a string from the config file
 *  @param name The name of the configuration item (e.g. config.network.port)
 *  @param defaultval The default value to be given if the config item doesn't exist
 *  @return The string configuration value
 */
string Configurator::get_config_string(string name, string defaultval)
{
	XMLTree *curtree;
	curtree=maintree;
	string ret=defaultval;
	do
	{
		int index=name.find('.');
		string subsec=name.substr(0,index);
		if (index!=-1) name.erase(0,index+1);
			else name.erase(0,name.size());
		curtree=curtree->get_subnode(subsec);
		if (curtree==NULL) break;
	}
	while (!name.empty());
	if (curtree)
	{
		ret=curtree->get_text();
	}
	return ret;
}

/** \brief Retrieve an int from the config file
 *  @param name The name of the configuration item (e.g. config.network.port)
 *  @param defaultval The default value to be given if the config item doesn't exist
 *  @return The int configuration value
 */
int Configurator::get_config_int(string name, int defaultval)
{
	string ret=get_config_string(name,"");
	if (ret.empty()) return defaultval;
		else return atoi(ret.c_str());
}

/** \brief Retrieve a double from the config file
 *  @param name The name of the configuration item (e.g. config.network.port)
 *  @param defaultval The default value to be given if the config item doesn't exist
 *  @return The double configuration value
 */
double Configurator::get_config_double(string name, double defaultval)
{
	string ret=get_config_string(name,"");
	if (ret.empty()) return defaultval;
		else return strtod(ret.c_str(),NULL);
}

/** \brief Retrieve a bool from the config file
 *  @param name The name of the configuration item (e.g. config.network.port)
 *  @param defaultval The default value to be given if the config item doesn't exist
 *  @return The bool configuration value
 */
bool Configurator::get_config_bool(string name, bool defaultval)
{
	string ret=get_config_string(name,"");
	if (ret.empty())
	{
		return defaultval;
	}
	else
	{
		transform(ret.begin(),ret.end(),ret.begin(),(int(*)(int))tolower);
		if (ret=="no" || ret=="false" || ret=="f" || ret=="n" || ret=="0")
			return false;
		else
			return true;
	}
}

void Configurator::set_config_string(string name, string defaultval)
{
	XMLTree *curtree,*nexttree;
	curtree=maintree;
	string ret=defaultval;
	do
	{
		int index=name.find('.');
		string subsec=name.substr(0,index);
		if (index!=-1) name.erase(0,index+1);
			else name.erase(0,name.size());
		nexttree=curtree->get_subnode(subsec);
		if (nexttree==NULL)
		{
			XMLTree *newtree=new XMLTree(subsec,Parser::AttributeMap());
			curtree->add_subnode(newtree);
			curtree=newtree;
		}
		else curtree=nexttree;
	}
	while (!name.empty());
	if (curtree)
	{
		curtree->set_text(defaultval);
	}
}

void Configurator::set_config_int(string name, int defaultval)
{
	ostringstream str;
	str << defaultval;
	set_config_string(name,str.str());
}

void Configurator::set_config_double(string name, double defaultval)
{
	ostringstream str;
	str << defaultval;
	set_config_string(name,str.str());
}

void Configurator::set_config_bool(string name, bool defaultval)
{
	if (defaultval)
		set_config_string(name,"true");
	else
		set_config_string(name,"false");
}

string Configurator::get_logger_filename()
{
	return get_config_string("config.logger.filename","/tmp/comrade.log");
}

string Configurator::get_logger_priority()
{
	return get_config_string("config.logger.priority","DEBUG");
}

string Configurator::get_network_group()
{
	return get_config_string("config.network.group","239.239.239.239");
}

string Configurator::get_network_port()
{
	return get_config_string("config.network.port","23932");
}

string Configurator::get_network_device()
{
	return get_config_string("config.network.device","eth0");
}

int Configurator::get_network_rxbuffersize()
{
	return get_config_int("config.network.rxbuffersize",4096);
}

int Configurator::get_network_txpacketsize()
{
	return get_config_int("config.network.txpacketsize",1024);
}

string Configurator::get_serial_device()
{
	return get_config_string("config.serial.device","/dev/ttyS0");
}

int Configurator::get_serial_rxbuffersize()
{
	return get_config_int("config.serial.rxbuffersize",1024);
}

int Configurator::get_serial_baudrate()
{
	return get_config_int("config.serial.baudrate",19200);
}

int Configurator::get_serial_retx_timeout()
{
	return get_config_int("config.serial.retransmission_timeout",100);
}

int Configurator::get_serial_max_framesize()
{
	return get_config_int("config.serial.max_framesize",16);
}

string Configurator::get_mouse_device()
{
	return get_config_string("config.mouse.device","/dev/psaux");
}

int Configurator::get_mouse_updateinterval()
{
	return get_config_int("config.mouse.updateinterval",0);
}

bool Configurator::get_mouse_exchange_lr()
{
	return get_config_bool("config.mouse.exchange_lr",false);
}

int Configurator::get_motorspeeds_leftforward()
{
	return get_config_int("config.motorspeeds.leftforward",200);
}

int Configurator::get_motorspeeds_leftbackward()
{
	return get_config_int("config.motorspeeds.leftbackward",200);
}

int Configurator::get_motorspeeds_rightforward()
{
	return get_config_int("config.motorspeeds.rightforward",200);
}

int Configurator::get_motorspeeds_rightbackward()
{
	return get_config_int("config.motorspeeds.rightbackward",200);
}

int Configurator::get_motorspeeds_minleftforward()
{
	return get_config_int("config.motorspeeds.minleftforward",0);
}

int Configurator::get_motorspeeds_minleftbackward()
{
	return get_config_int("config.motorspeeds.minleftbackward",0);
}

void Configurator::set_motorspeeds_minleftforward(int speed)
{
	set_config_int("config.motorspeeds.minleftforward",speed);
}

void Configurator::set_motorspeeds_minleftbackward(int speed)
{
	set_config_int("config.motorspeeds.minleftbackward",speed);
}

int Configurator::get_motorspeeds_minrightforward()
{
	return get_config_int("config.motorspeeds.minrightforward",0);
}

int Configurator::get_motorspeeds_minrightbackward()
{
	return get_config_int("config.motorspeeds.minrightbackward",0);
}

void Configurator::set_motorspeeds_minrightforward(int speed)
{
	set_config_int("config.motorspeeds.minrightforward",speed);
}

void Configurator::set_motorspeeds_minrightbackward(int speed)
{
	set_config_int("config.motorspeeds.minrightbackward",speed);
}

void Configurator::set_motorspeeds_leftforward(int speed)
{
	set_config_int("config.motorspeeds.leftforward",speed);
}

void Configurator::set_motorspeeds_leftbackward(int speed)
{
	set_config_int("config.motorspeeds.leftbackward",speed);
}

void Configurator::set_motorspeeds_rightforward(int speed)
{
	set_config_int("config.motorspeeds.rightforward",speed);
}

void Configurator::set_motorspeeds_rightbackward(int speed)
{
	set_config_int("config.motorspeeds.rightbackward",speed);
}

double Configurator::get_odometry_wheelbase()
{
	return get_config_double("config.odometry.wheelbase",1000);
}

double Configurator::get_odometry_translationconstant()
{
	return get_config_double("config.odometry.translationconstant",1000);
}

void Configurator::get_odometry_wheelbase(double wheelbase)
{
	set_config_double("config.odometry.wheelbase",wheelbase);
}

void Configurator::get_odometry_translationconstant(double constant)
{
	set_config_double("config.odometry.translationconstant",constant);
}

bool Configurator::get_motorspeeds_leftinvert()
{
	return get_config_bool("config.motorspeeds.leftinvert",false);
}

bool Configurator::get_motorspeeds_rightinvert()
{
	return get_config_bool("config.motorspeeds.rightinvert",false);
}

bool Configurator::get_mouse_leftinvert()
{
	return get_config_bool("config.mouse.leftinvert",false);
}

bool Configurator::get_mouse_rightinvert()
{
	return get_config_bool("config.mouse.rightinvert",false);
}

int Configurator::get_compass_minvalue()
{
	return get_config_int("config.compass.minvalue",10);
}

int Configurator::get_compass_maxvalue()
{
	return get_config_int("config.compass.maxvalue",510);

}

void Configurator::set_compass_minvalue(int value)
{
	set_config_int("config.compass.minvalue",value);
}

void Configurator::set_compass_maxvalue(int value)
{
	set_config_int("config.compass.maxvalue",value);
}
