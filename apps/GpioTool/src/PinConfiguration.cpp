#include "PinConfiguration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

PinConfiguration::PinConfiguration(Application & application)
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
							scene_collection()->set_current_scene("Configuration");
						}
					}
				})
			);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);

	add_component(
				"VerticalLayout",
				(* new ux::Layout())

				.add_component(
					"FunctionLabel",
					(* new ux::Label())
					.set_label("Function")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"FunctionValue",
					(* new ux::Label())
					.set_label("Uart::Tx")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"FunctionValue",
					(* new ux::Label())
					.set_label("Uart::Rx")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"FunctionValue",
					(* new ux::Label())
					.set_label("Uart::CTS")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"FunctionValue",
					(* new ux::Label())
					.set_label("Uart::RTS")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"Button",
					(* new ux::Button())
					.set_label("Test")
					.set_border_size(1)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					.set_event_handler([this](Component * object, const Event & event){

						if( (event.type() == ButtonEvent::event_type()) &&
						(event.id() == ButtonEvent::id_pressed) ){
							printf("pressed\n");
						}
					})
					)
				.set_drawing_point(
					DrawingPoint(
						25, 225
						)
					)
				.set_drawing_area(
					DrawingArea(950,775)
					)

				.set_event_handler([this](Component * object, const Event & event){

					if( (event.type() == SystemEvent::event_type()) &&
					(event.id() == SystemEvent::id_update) ){

					}


				})

				);


}



