#include "Display.hpp"
#include "Application.hpp"

#include <ToolboxAPI/components/TopNavigation.hpp>
#include <sapi/ux.hpp>

Display::Display(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	add_component(
				"DisplayTopNavigation",
				(* new toolbox::TopNavigation("Display", "BackHome", event_loop()))
				.set_drawing_area(DrawingArea(1000,175))
				);

#if 0
	add_component(
				"SunButton",
				(* new Button())
				.set_icon_name("sun")
				.set_border_size(0)
				.set_theme_style(Theme::style_outline_warning)
				.set_drawing_point(DrawingPoint(250,200))
				.set_drawing_area(DrawingArea(500,500))
				);


	add_component(
				"BrightnessSlider",
				(* new ux::Slider())
				.set_value(25)
				.set_maximum(100)
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(ux::DrawingPoint(100,750))
				.set_drawing_area(ux::DrawingArea(800,100))
			);
#endif

}



