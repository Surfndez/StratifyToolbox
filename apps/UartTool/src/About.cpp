#include "About.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

About::About(Application * application)
	: ApplicationLayout("About", application){

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
				ApplicationDetails::create(
					"UartTool",
					event_loop()
					)
				.set_drawing_area(DrawingArea(1000,1000-175))
				);

}

void About::local_event_handler(const Event & event){
	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button &&  button->name() ==
			find<TopNavigation>(top_navigation_name())->left_button_name()
			){
		event_loop()->layout()->transition("Control");
	}
}




