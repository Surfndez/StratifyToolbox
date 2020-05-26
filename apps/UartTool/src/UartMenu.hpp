#ifndef UART_MENU_HPP
#define UART_MENU_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include "Menu.hpp"

class Application;

class UartMenu : public ApplicationLayout<UartMenu> {
public:
	 UartMenu(const var::String& name, Application * app);

	var::String top_navigation_name() const {
		 return (name() + "UartMenuTopNavigation").cstring();
	 }

	 void local_event_handler(const ux::Event & event);

private:

};

#endif // UART_MENU_HPP
