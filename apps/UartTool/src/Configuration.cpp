#include "Configuration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

#include "UartMenu.hpp"
#include "Menu.hpp"


Configuration::Configuration(Application * app)
	: ApplicationLayout<Configuration>("Configuration", app){


	add_component(
				Menu::create("Settings", event_loop())
				.add_item(
							MenuItem::create("Local")
							.set_type(MenuItem::type_menu)
							)
				.add_item(
							MenuItem::create("USB")
							.set_type(MenuItem::type_menu)
							)
				.add_item(
							MenuItem::create("Wifi")
							.set_type(MenuItem::type_menu)
							)
				.add_filler()
				);


	add_component(
				UartMenu::create("Local", app)
				.set_enabled(false)
				);

	add_component(
				UartMenu::create("USB", app)
				.set_enabled(false)
				);

	add_component(
				UartMenu::create("Wifi", app)
				.set_enabled(false)
				);

}

void Configuration::local_event_handler(
		const ux::Event & event
		){

	//check for button push to exit back to LocalTerminal
	Button * button = ButtonEvent::component(event);
	if( button ){

		if( event.id() == ButtonEvent::id_released ){
			printer().info("Handle Settings Menu button press: "	+ button->name() + " for " + name());

			if( button->name() == "SettingsTopNavigationLeftButton" ){
				parent()->transition("LocalTerminal");
			}

		} else if( event.id() == ButtonEvent::id_held ){

		}

	}

}






