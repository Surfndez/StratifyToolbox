#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components/TopNavigation.hpp>

#include "About.hpp"
#include "Application.hpp"

About::About(Application & application)
	: toolbox::ApplicationLayout<Application>(application){


	SysInfo info = Sys::get_info();

	add_component(
				"DisplayTopNavigation",
				(* new toolbox::TopNavigation("About", "BackHome", event_loop()))
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				"VersionLabel",
				(* new Label())
				.set_label("Version")
				.set_border_size(0)
				.set_align_left()
				.set_theme_style(Theme::style_light)
				.set_drawing_point(DrawingPoint(20,200))
				.set_drawing_area(DrawingArea(980,100))
				);

	add_component(
				"Version",
				(* new Label())
				.set_label(info.bsp_version() + "." + info.kernel_version())
				.set_border_size(0)
				.set_align_left()
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_drawing_point(DrawingPoint(20,300))
				.set_drawing_area(DrawingArea(980,150))
				);







}



