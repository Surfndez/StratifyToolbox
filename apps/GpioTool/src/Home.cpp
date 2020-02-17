#include "Home.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	add_component(
				"ControlButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_label("IO")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(150, 150))
				.set_drawing_area(DrawingArea(300, 300))
				);

	add_component(
				"ConfigurationButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("sliders-h")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(550, 150))
				.set_drawing_area(DrawingArea(300, 300))
				);

	add_component(
				"AboutButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("info")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(150, 550))
				.set_drawing_area(DrawingArea(300, 300))
				);

	add_component(
				"ExitButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_label("EXIT")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(550, 550))
				.set_drawing_area(DrawingArea(300, 300))
				);

}



