#include "Control.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Control::Control(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);


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
							scene_collection()->set_current_scene("Home");
						}
					}


				})
				);

	const u32 rows = 3;
	const u32 columns = 4;

	for(u32 row=0; row < rows; row++){
		for(u32 column=0; column < columns; column++){
			String label;
			label.format("io%d", row*columns + column);
			add_component(
						label,
						(* new ux::Button())
						.set_border_size(1)
						.set_label(label)
						.set_theme_style(static_cast<Theme::style>(Theme::style_brand_primary + row))
						.set_drawing_point(DrawingPoint(column*250+25, 200 + row*225+25))
						.set_drawing_area(DrawingArea(200, 175))
						);
		}
	}



}



