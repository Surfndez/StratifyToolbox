#include "PinConfiguration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/Io.hpp>

PinConfiguration::PinConfiguration(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	add_component(
				"BackConfiguration",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("chevron-left")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(0, 0))
				.set_drawing_area(DrawingArea(175, 175))
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
						25, 225
						)
					)
				.set_drawing_area(
					DrawingArea(950,775)
					)

				);

	set_event_handler([&application, this](Component * object, const Event & event){

		if( event.type() == SystemEvent::event_type() ){
			if( event.id() == SystemEvent::id_update ){

				String pin_configuration =
						application.model().at("pinConfiguration").to_string();

				if( pin_configuration.is_empty() == false ){
					application.model().remove("pinConfiguration");
					var::Vector<String> items = pin_configuration.split("-");
					if( items.count() > 1 ){
						toolbox::IoInformation info(items.at(1));
						toolbox::Io io(info.io_pin());

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
		}
	});


}



