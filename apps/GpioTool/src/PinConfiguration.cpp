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
				.set_vertical_scroll_enabled()

				.add_component(
					"PinLabel",
					(* new ux::Label())
					.set_label("Pin")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"PinValue",
					(* new ux::Label())
					.set_label("?")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					.set_event_handler([&, this](Component * object, const Event & event){
		if( (event.type() == SystemEvent::event_type()) ){
			if( event.id() == SystemEvent::id_enter ){
				//update the label if pin is input/output
				const String pin_name = application.model().at("pinName").to_string();
				Label * label = static_cast<Label*>(object);
				label->set_label(pin_name);
				application.printer().info("select pin %s", pin_name.cstring());
				label->redraw();
			}
		}
	})
					)

				.add_component(
					"ValueLabel",
					(* new ux::Label())
					.set_label("Value")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"ValueValue",
					(* new ux::Label())
					.set_label("---")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					.set_event_handler([&, this](Component * object, const Event & event){
		if( (event.type() == SystemEvent::event_type()) ){
			if( event.id() == SystemEvent::id_update ){
				//update the label if pin is input/output
				Label * label = static_cast<Label*>(object);

				toolbox::IoInformation io_information(
							application.model().at("pinName").to_string()
							);

				toolbox::Io io_pin(io_information.io_pin());
				label->set_label(
							io_pin.value() ? "high" : "low"
															 );
				label->redraw();
			}
		}
	})
					)

				.add_component(
					"DirectionLabel",
					(* new ux::Label())
					.set_label("Direction")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"DirectionLayout",
					(* new ux::Layout())
					.set_flow(Layout::flow_horizontal)

					.add_component(
						"DirectionValue",
						(* new ux::Label())
						.set_label("Output")
						.set_align_left()
						.set_border_size(0)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(600,1000))
						.set_event_handler([&, this](Component * object, const Event & event){

		if( (event.type() == SystemEvent::event_type()) ){

			if( event.id() == SystemEvent::id_enter ){
				String pin_name = application.model().at("pinName").to_string();
				//update the label if pin is input/output
				application.printer().info("using pin %s",
																	 pin_name.cstring()
																	 );

				Label * label = (Label*)object;
				label->set_label(
							toolbox::Io(toolbox::IoInformation(pin_name).io_pin()).is_output() ?
								"output" :
								"input"
						);
				label->redraw();


			}

			if( event.id() == SystemEvent::id_update ){
				//update the label if pin is input/output
			}

		}

	})
						)

					.add_component(
						"DirectionButton",
						(* new ux::Button())
						.set_label("Toggle")
						.set_border_size(1)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(400,1000))
						.set_event_handler([this](Component * object, const Event & event){

							if( event.type() == ButtonEvent::event_type() ){
								const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

								if( button_event.id() == ButtonEvent::id_released ){
									//toggle direction
									printer().info("toggle button");

								}
							}

						})
					)
				.set_drawing_area(value_area)
				)

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
				.set_label("GPIO")
				.set_align_left()
				.set_border_size(0)
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_drawing_area(value_area)
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



