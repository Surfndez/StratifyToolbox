#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

#include "Settings.hpp"
#include "Application.hpp"
#include "Display.hpp"
#include "About.hpp"



Settings::Settings(Application & application)
	: toolbox::ApplicationLayout<Application>("Settings", application){

	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_right_icon_name("info")
					.set_left_icon_name("times")
					.set_title("Settings"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				(* new List("List",event_loop()))
				.set_item_height(250)
				.set_vertical_scroll_enabled()
				.add_component(

					(* new ListItem("Account"))
					.set_key("Account")
					.set_value("tgil")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					(* new ListItem("Display"))
					.set_key("Display")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					(* new ListItem("Apps"))
					.set_key("Apps")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					(* new ListItem("Network"))
					.set_key("Network")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					(* new ListItem("Files"))
					.set_key("Files")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					(* new ListItem("Battery"))
					.set_key("Battery")
					.set_icon("chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.set_drawing_point(DrawingPoint(0,175))
				.set_drawing_area(DrawingArea(1000,1000-175))
				);

	set_event_handler(Settings::event_handler);
}


void Settings::local_event_handler(const Event & event){

	if( event.type() == ListEvent::event_type() ){
		const ListEvent & list_event = event.reinterpret<ListEvent>();
		if( list_event.item().name() == "Display" ){
			event_loop()->layout()->transition("Display");
		}
	}

	if( ButtonEvent::is_event(event, ButtonEvent::id_released) ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
		if( button_event.name() ==
				find<TopNavigation>(top_navigation_name())->left_button_name() ){
			application().go_home();
		} else if( button_event.name() ==
							 find<TopNavigation>(top_navigation_name())->right_button_name() ){
			event_loop()->layout()->transition("About");
		}
	}

}


