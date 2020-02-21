#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ToolboxAPI/toolbox.hpp>
#include <sapi/sys/Cli.hpp>
#include <sapi/ux.hpp>

class Application : public ToolboxApplication {

public:
   Application(const sys::Cli & cli);

	 ux::Layout& create_layout();

private:

	 static void handle_application_event(
			 Application & application,
			 ux::Component * object,
			 const ux::Event & event
			 );

	 void toggle_direction();

};

#endif // APPLICATION_HPP
