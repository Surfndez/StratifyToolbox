#include "Configuration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

#include "Menu.hpp"


Configuration::Configuration(Application * app)
	: ApplicationLayout("Configuration", app){

	const StringList parity_options = {
		"none", "odd", "even"
	};
	PRINTER_TRACE(printer(), "construct");

	add_component(
				Menu::create("Settings", event_loop())
				.add_item(
					MenuItem::create("Serial Settings")
					.set_type(MenuItem::type_menu)
					)
				.add_item(
					MenuItem::create("Wifi")
					.set_type(MenuItem::type_bool)
					.set_checked()
					)
				.add_item(
					MenuItem::create("Parity")
					.set_present_value("none")
					.set_type(MenuItem::type_string_list)
					)
				.add_filler()
				);

	PRINTER_TRACE(printer(), "data");
	printer().object("after settings", DataInfo());
	add_component(
				Menu::create("Serial Settings", event_loop())
				.add_item(
					MenuItem::create("USB")
					.set_type(MenuItem::type_bool)
					.set_checked()
					)
				.add_filler()
				.set_caller("Settings")
				.set_enabled(false)
				);

	printer().object("after serial settings", DataInfo());

	add_component(
				Menu::create_options_list(
					"Parity", event_loop(), "none", parity_options
					)
				.add_filler()
				.set_caller("Settings")
				.set_enabled(false)
				);
	printer().object("after parity", DataInfo());
}

void Configuration::local_event_handler(
		const ux::Event & event
		){

}






