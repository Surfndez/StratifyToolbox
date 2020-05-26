#include "UartMenu.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

#include "Menu.hpp"


UartMenu::UartMenu(
		const var::String& name,
		Application * app)
	: ApplicationLayout(name, app){

	add_component(
				Menu::create(name, event_loop())
				.add_item(
					MenuItem::create("Uart")
					.set_type(MenuItem::type_menu)
					)
				.add_item(
					MenuItem::create("Line Settings")
					.set_present_value("manual")
					.set_type(MenuItem::type_string_list)
					)
				.add_item(
					MenuItem::create("BaudrateValue")
					.set_key("Baudrate")
					.set_target_name("BaudrateInput")
					.set_present_value("115200")
					.set_type(MenuItem::type_string)
					)
				.add_item(
					MenuItem::create("Parity")
					.set_present_value("none")
					.set_type(MenuItem::type_string_list)
					)
				.add_item(
					MenuItem::create("Stop Bits")
					.set_present_value("1")
					.set_type(MenuItem::type_string_list)
					)
				.add_filler()

				);

	const StringList line_mode_options = {
		"auto", "manual"
	};

	const StringList parity_options = {
		"none", "odd", "even"
	};

	const StringList stop_bit_options = {
		"1", "1.5", "2"
	};

	add_component(
				Keyboard::create("BaudrateInput", event_loop())
				.set_caller(name)
				.set_target( find<MenuItem>("BaudrateValue")->present_value_pointer() )
				.set_enabled(false)
				);

	add_component(
				Menu::create_options_list(
					"Line Settings", event_loop(), "auto", line_mode_options
					)
				.set_caller(name)
				.set_enabled(false)
				);

	add_component(
				Menu::create_options_list(
					"Parity", event_loop(), "none", parity_options
					)
				.set_caller(name)
				.set_enabled(false)
				);


	add_component(
				Menu::create_options_list(
					"Stop Bits", event_loop(), "1", stop_bit_options
					)
				.set_caller(name)
				.set_enabled(false)
				);


}

void UartMenu::local_event_handler(
		const ux::Event & event
		){
	Button * button = ButtonEvent::component(event);
	if( button ){

		if( event.id() == ButtonEvent::id_released ){
			printer().info("Handle Uart Menu button press: "	+ button->name() + " for " + name());

			if( button->name() == name() + "TopNavigationLeftButton" ){
				parent()->transition("Settings");
			}

		} else if( event.id() == ButtonEvent::id_held ){

		}

	}
}






