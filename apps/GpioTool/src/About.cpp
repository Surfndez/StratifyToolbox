#include "About.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

About::About(Application & application)
	: ApplicationLayout<Application>("About", application){

	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_title("About"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	set_event_handler(
				About::event_handler
				);


}

void About::local_event_handler(const Event & event){
	if( (event.type() == ButtonEvent::event_type()) &&
			(event.id() == ButtonEvent::id_released)
			){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
		if( button_event.name() ==
				find<TopNavigation>(top_navigation_name())->left_button_name()
				){
			event_loop()->layout()->transition("Control");
		}
	}
}




