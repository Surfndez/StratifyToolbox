#ifndef MENU_HPP
#define MENU_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Menu : public ApplicationLayout<Menu> {
public:
	 Menu(Application * app);

	 static const char * top_navigation_name(){
		 return "MenuTopNavigation";
	 }

	 void local_event_handler(const ux::Event & event);

private:

};

#endif // MENU_HPP
