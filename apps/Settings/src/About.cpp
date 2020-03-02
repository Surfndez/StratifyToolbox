#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

#include "About.hpp"
#include "Application.hpp"

About::About(Application & application)
	: toolbox::ApplicationLayout<Application>("About", application){

	SysInfo info = Sys::get_info();
	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_title("About"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				Component::create<Label>("VersionLabel")
				.set_label("Version")
				.set_border_size(0)
				.set_align_left()
				.set_theme_style(Theme::style_light)
				.set_drawing_point(DrawingPoint(20,200))
				.set_drawing_area(DrawingArea(980,100))
				);

	add_component(
				Component::create<Label>("Version")
				.set_label(info.bsp_version() + "." + info.kernel_version())
				.set_border_size(0)
				.set_align_left()
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_drawing_point(DrawingPoint(20,300))
				.set_drawing_area(DrawingArea(980,150))
				);

	set_event_handler(About::event_handler);
}

void About::local_event_handler(const Event & event){

	if( (event.type() == ButtonEvent::event_type()) &&
			(event.id() == ButtonEvent::id_released) ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
		if( button_event.name() ==
				find<TopNavigation>(top_navigation_name()
					)->left_button_name() ){
			event_loop()->layout()->transition("Settings");
		}
	}

}




