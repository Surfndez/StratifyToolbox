#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

#include "About.hpp"
#include "Application.hpp"

About::About(Application* application)
	: toolbox::ApplicationLayout<About>("About", application){

	SysInfo info = Sys::get_info();

	set_flow(flow_vertical);

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_title("About"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				Label::create("VersionLabel")
				.set_label("Version")
				.set_border_size(0)
				.set_align_left()
				.set_horizontal_margin(10)
				.set_theme_style(Theme::style_light)
				.set_drawing_area(DrawingArea(1000,100))
				);

	add_component(
				Label::create("Version")
				.set_label(info.bsp_version() + "." + info.kernel_version())
				.set_border_size(0)
				.set_align_left()
				.set_align_top()
				.set_horizontal_margin(10)
				.set_bottom_margin(100)
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_drawing_area(DrawingArea(1000,1000-175-100))
				);

	set_event_handler(About::event_handler);
}

void About::local_event_handler(const Event & event){
	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button && button->name() ==
			find<TopNavigation>(top_navigation_name()
				)->left_button_name()
			){
		event_loop()->layout()->transition("Settings");
	}
}




