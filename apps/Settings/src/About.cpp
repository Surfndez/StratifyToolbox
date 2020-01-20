#include <sapi/sys.hpp>

#include "TopNavigation.hpp"
#include "About.hpp"
#include "Application.hpp"

About::About(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);

	SysInfo info = Sys::get_info();

	add_component(
				"TopNavigation",
				(* new TopNavigation("About"))
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



