#include "About.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

About::About(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	event_loop()->set_update_period(chrono::Milliseconds(10));

	add_component(
				"BackButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("chevron-left")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(0, 0))
				.set_drawing_area(DrawingArea(200, 200))
				.set_event_handler([&](Component * object, const Event & event){

					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "BackButton" ){
							//scene_collection()->set_current_scene("Home");

						}
					}


				})
				);



}



