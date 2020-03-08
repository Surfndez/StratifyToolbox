#include "Home.hpp"
#include "Application.hpp"
#include "WidgetWindow.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application* application)
	: toolbox::ApplicationLayout<Home>("Home", application){

	const ux::DrawingArea control_icon_area(1000,250);
	const u8 control_border_size = 0;
	const enum Theme::style control_icon_style = Theme::style_brand_primary;

	add_component(
				Layout::create("ButtonLayout", event_loop())
				.set_flow(ux::Layout::flow_vertical)
				.set_drawing_area(DrawingArea(250, 1000))
				.set_drawing_point(DrawingPoint(750,0))
				.add_component(
					Button::create("WifiButton")
					.set_border_size(control_border_size)
					.set_icon_name("wifi")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
					)
				.add_component(
					Button::create("LaunchButton")
					.set_border_size(control_border_size)
					.set_icon_name("rocket")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
					)
				.add_component(
					Button::create("EjectButton")
					.set_border_size(control_border_size)
					.set_icon_name("eject")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
					)
				.add_component(
					Button::create("SettingsButton")
					.set_border_size(control_border_size)
					.set_icon_name("wrench")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
					)
				);

	set_event_handler(event_handler);
}

void Home::local_event_handler(const Event & event){
	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button ){
		if( button->name() == "SettingsButton" ){
			application()->launch("/home/Settings");
		} else if( button->name() == "LaunchButton" ){
			application()->launch("/home/Launcher");
		}
	}
}
