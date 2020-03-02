#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/ToolboxApplication.hpp>

class Application;


class Settings : public toolbox::ApplicationLayout<Application> {
public:
   Settings(Application & application);


	 static const char * top_navigation_name(){
		 return "SettingsTopNavigation";
	 }

private:
	 static void event_handler(
			 Component * object,
			 const Event & event){
		 object->reinterpret<Settings>()->local_event_handler(event);
	 }

	 void local_event_handler(const Event & event);

};

#endif // HOME_HPP
