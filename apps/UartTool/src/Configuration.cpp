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




	add_component(
				Menu::create("Settings", event_loop())
				.add_item(
					MenuItem::create("Serial")
					.set_key("Serial Settings")
					.set_type(MenuItem::type_menu)
					.set_value("icon@chevron-right")
					)
				.add_item(
					MenuItem::create("Wifi")
					.set_key("Wifi")
					.set_type(MenuItem::type_bool)
					.set_checked()
					)
				.add_item(
					MenuItem::create("Parity")
					.set_key("Parity")
					.set_type(MenuItem::type_string_list)
					)
				.	add_component(
					Menu::create_options_list(
						"Parity", event_loop(), "none", parity_options
						)
					.set_enabled(false)
					)
				);

#if 0
	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("Uart Tool"),
					event_loop())
				);

	add_component(
				List::create("List", event_loop())
				.set_item_height(250)
				.set_vertical_scroll_enabled()
				.add_component(
					ListItem::create("Serial")
					.set_key("Serial Settings")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("Wifi")
					.set_key("Wifi")
					.set_value("icon@check")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("USB")
					.set_key("USB")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("SDCard")
					.set_key("SD Card")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("Help")
					.set_key("Help")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.set_drawing_point(DrawingPoint(0,175))
				.set_drawing_area(DrawingArea(1000,1000-175))
				);
#endif

}

void Configuration::local_event_handler(
		const ux::Event & event
		){


}






