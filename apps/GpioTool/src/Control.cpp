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
			u32 pin = row*columns + column;
			String label;
			label.format("io%d", pin);
			add_component(
						label,
						(* new ux::Button())
						.set_border_size(1)
						.set_label(label)
						.set_theme_style(static_cast<Theme::style>(pin % Theme::last_style))
						.set_drawing_point(DrawingPoint(column*250+25, 200 + row*225+25))
						.set_drawing_area(DrawingArea(200, 175))
						);
		}
	}

	const DrawingArea pin_area(45,40);
	const u32 pins = 20;
	for(u32 pin=0; pin < pins; pin++){
		add_component(
					String().format("%d", pin),
					(* new ux::Label())
					.set_border_size(1)
					.set_theme_style(static_cast<Theme::style>(pin % Theme::last_style))
					.set_drawing_point(DrawingPoint(pin*pin_area.width()+50, 1000-pin_area.height()))
					.set_drawing_area(pin_area)
					);
	}



}



