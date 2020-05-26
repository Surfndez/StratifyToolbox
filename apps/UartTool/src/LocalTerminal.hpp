#ifndef LOCAL_TERMINAL_HPP
#define LOCAL_TERMINAL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class LocalTerminal : public ApplicationLayout<LocalTerminal> {
public:
	 LocalTerminal(Application * app);

	 static const var::String pin_button_name(const IoInfo & information);
	 static const var::String pin_marker_name(const IoInfo & information);

	 static const char * top_navigation_name(){
		 return "LocalTerminalTopNavigation";
	 }

	 void local_event_handler(const ux::Event & event);

private:


};

#endif // LOCAL_TERMINAL_HPP
