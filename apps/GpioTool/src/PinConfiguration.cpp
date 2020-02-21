#include "PinConfiguration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/Io.hpp>
#include <ToolboxAPI/components/TopNavigation.hpp>

PinConfiguration::PinConfiguration(Application & application)
	: ApplicationLayout<Application>(application){


	const drawing_size_t navigation_bar_height = 175;
	add_component(
				"ConfigurationTopNavigation",
				(* new TopNavigation("Pin Configuration", "BackConfiguration", event_loop()))
				.set_drawing_area(DrawingArea(1000,navigation_bar_height))
				);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);

	add_component(
				"PinConfigurationVerticalLayout",
				(* new ux::Layout(event_loop()))
				.set_flow(ux::Layout::flow_vertical)
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
					(* new ux::Layout(event_loop()))
					.set_flow(Layout::flow_horizontal)

					.add_component(
						"DirectionValue",
						(* new ux::Label())
						.set_label("Output")
						.set_align_left()
						.set_border_size(0)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(600,1000))
						)

					.add_component(
						"DirectionButton",
						(* new ux::Button())
						.set_label("Toggle")
						.set_border_size(1)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(400,1000))
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
						25, navigation_bar_height
						)
					)
				.set_drawing_area(
					DrawingArea(950,1000-navigation_bar_height)
					)

				);

	set_event_handler([&application, this](Component * object, const Event & event){

		if( event.type() == SystemEvent::event_type() ){
			if( event.id() == SystemEvent::id_update ){
				update_display_values();
			}
		}

		if( (event.type() == ButtonEvent::event_type()) &&
				(event.id() == ButtonEvent::id_released) ){
			const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

			if( button_event.name() == "DirectionButton" ){
				toggle_direction();
			}

		}

	});


}

void PinConfiguration::toggle_direction(){
	Label * pin_value_label = find<Label>("PinValue");
	if( pin_value_label == nullptr ){
		//assert here
		return;
	}

	IoInformation information(pin_value_label->label());
	if( information.is_valid() ){
		application().printer().info("Toggle IO for " + information.name());
		Io io(information.io_pin());

		Label * direction_label = find<Label>("DirectionValue");
		if( io.is_output() ){
			io.set_input();
			direction_label->set_label("Input");
		} else {
			io.set_output();
			direction_label->set_label("Output");
		}
		direction_label->redraw();
	}

}

void PinConfiguration::update_display_values(){
	String pin_configuration =
			application().model().at("pinConfiguration").to_string();

	if( pin_configuration.is_empty() == false ){
		application().model().remove("pinConfiguration");
		var::Vector<String> items = pin_configuration.split("-");
		if( items.count() > 1 ){
			IoInformation info(items.at(1));
			Io io(info.io_pin());

			this->find<Label>(
						"PinValue"
						)->set_label(
						info.name()
						).redraw();

			this->find<Label>(
						"ValueValue"
						)->set_label(
						io.value() ? "High" : "Low"
												 ).redraw();

			this->find<Label>(
						"DirectionValue"
						)->set_label(
						io.is_output() ? "Output" : "Input"
														 ).redraw();

			this->find<Label>(
						"FunctionValue"
						)->set_label(
						"Uart::Tx"
						).redraw();

		}
	}
}




