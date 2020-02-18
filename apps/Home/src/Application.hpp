#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ToolboxAPI/Application.hpp>
#include <sapi/sys/Cli.hpp>
#include <sapi/ux/Layout.hpp>

#include "Home.hpp"

class Application : public toolbox::Application {

public:
   Application(const sys::Cli & cli);

	 ux::Layout& create_layout();

private:

	 static void handle_application_event(
			 Application & application,
			 ux::Component * object,
			 const ux::Event & event
			 );


};

#endif // APPLICATION_HPP
