#include "App.h"

const std::string App::ScanPorts()
{
	std::vector<serial::PortInfo> devices_found = serial::list_ports();

	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

	std::string res;
	while( iter != devices_found.end() )
	{
		serial::PortInfo device = *iter++;
		res += device.port + ", " + device.description + "\n";
	}
	return res;
}
const std::vector<std::string> App::GetComAvailable()
{
	std::vector<serial::PortInfo> devices_found = serial::list_ports();

	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

	std::vector<std::string> res;
	while( iter != devices_found.end() )
	{
		serial::PortInfo device = *iter++;
		res.push_back( device.port );
	}
	return res;
}	
const bool App::CheckCOM( const std::string& com_name )
{
	const std::vector<std::string>& list_port = GetComAvailable();
	for( auto p : list_port )
	{
		if( p == com_name ) return true; 
	} 
	return false;
}
const bool App::Valid_InitCOM()
{
	cmd_title.clear();
	if( com_name == "" || !App::CheckCOM( com_name ) ) 
	{
		body = "Hay chon cong COM kha dung";
		return false;
	}
	return true;
}
const bool App::CheckBreakCOM()
{
	if( com_name == "" || !App::CheckCOM( com_name ) ) 
	{
		cmd = cmd_title.size();
		return true;
	}
	return false;
}
