#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

#include "Menu.hpp"


class Configuration :  public ApplicationLayout<Configuration> {
public:
	 Configuration(Application * app);

	 static const char * top_navigation_name(){
		 return "ConfigurationTopNavigation";
	 }

	 void local_event_handler(const ux::Event & event);

private:

};

#endif // CONFIGURATION_HPP
