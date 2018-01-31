#ifndef _BOOST_XML_SERIALIZATION_KYOSHO_2012_02_22_
#define _BOOST_XML_SERIALIZATION_KYOSHO_2012_02_22_
#include <string>
#include <map>

class Xml_serialization{
public:
	typedef std::map<std::string,std::string> m_value;
	typedef std::map<std::string,m_value> m_data;
    Xml_serialization(std::string file_name);
	Xml_serialization();
	~Xml_serialization();
	bool Load();
	bool Load(std::string file_name);
	bool Save();
	void AddData(std::string index,std::string name,std::string value);
	std::string GetData(std::string index,std::string name);

private:
	std::string file_name_;
	m_data data_;

};
#endif //_BOOST_XML_SERIALIZATION_KYOSHO_2012_02_22_
