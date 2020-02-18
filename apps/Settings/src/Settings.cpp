#include "Settings.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>

Settings::Settings(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	add_component(
				"List",
				(* new List(event_loop()))
				.set_item_height(250)
				.set_vertical_scroll_enabled()
				.add_component(
					"Account",
					(* new ListItem())
					.set_key("Account")
					.set_value("tgil")
					.set_icon("chevron-right")
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					"Display",
					(* new ListItem())
					.set_key("Display")
					.set_icon("chevron-right")
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					"Wifi",
					(* new ListItem())
					.set_key("Wifi")
					.set_icon("chevron-right")
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					"About",
					(* new ListItem())
					.set_key("About")
					.set_icon("chevron-right")
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					"Exit",
					(* new ListItem())
					.set_key("Exit")
					.set_icon("ellipsis-v")
					.set_theme_style(Theme::style_outline_dark)
					)
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(1000,1000))
			);

}



