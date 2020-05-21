
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "About.hpp"
#include "Control.hpp"
#include "Configuration.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli){
	printer().set_verbose_level(cli.get_option("verbose"));
}

ux::Layout & Application::create_layout(){
	PRINTER_TRACE(printer(), "create application layout");
	return Layout::create(
				"Application",
				event_loop()
				)
			//.add_component(Control::create(this).set_enabled(false))
			.add_component(Configuration::create(this).set_enabled(true))
			//.add_component(About::create(this).set_enabled(false))
			;

}


