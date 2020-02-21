#include "Control.hpp"
#include "Application.hpp"
#include "PinConfiguration.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

const var::String Control::pin_button_name(
		const IoInformation & information
		){
	return information.name();
}

const var::String Control::pin_marker_name(
		const IoInformation & information){
	return PinMarkerBar::pin_marker_name(information.io_pin());
}


Control::Control(Application & app)
	: ApplicationLayout<Application>(app){

	for(auto & value: m_pin_states){
		value = -1;
	}

	add_component(
				"ConfigurationTopNavigation",
				(* new TopNavigation("Control", "BackHome", event_loop()))
				.set_drawing_area(DrawingArea(1000,175))
				);


	const u32 columns = 4;

	var::Vector<IoInformation> io_information_list =
			Io::io_information_list(
				IoInformation::type_io
				);

	u32 row = 1;
	u32 column = 0;

	const DrawingArea button_area(225,200);
	const DrawingArea pin_area(45,40);

	add_component(
				"PinMarkerBar",
				(* new PinMarkerBar(
					 event_loop(),
					 IoInformation::type_io
					 )
				 )
				//bottom 10% of screen (full width)
				.set_drawing_point(DrawingPoint(0,900))
				.set_drawing_area(DrawingArea(1000,100))
				);

	PinMarkerBar * pin_marker_bar = find<PinMarkerBar>("PinMarkerBar");
	if( pin_marker_bar == nullptr ){
		application().printer().error("pin marker bar is null");
		exit(1);
	}

	for(const auto & info: io_information_list){
		enum Theme::style style;

		Io io(info.io_pin());
		if( io.is_output() ){
			style = Theme::style_brand_secondary;
		} else {
			style = Theme::style_outline_brand_secondary;
		}

		add_component(
					pin_button_name(info),
					(* new ux::Button())
					.set_border_size(1)
					.set_label(info.name())
					.set_theme_style(style)
					.set_drawing_point(
						DrawingPoint(
							column*(button_area.width()+25)+25/2,
							row*(button_area.height()+25)
							)
						)
					.set_drawing_area(button_area)
					);

		pin_marker_bar->set_pin_marker_style(
					info.io_pin(),
					style
					);

		column++;
		if( column == columns ){
			row++;
			column = 0;
		}
	}

	set_event_handler(Control::handle_event);

}

void Control::handle_control_event(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){

			var::Vector<IoInformation> io_information_list =
					Io::io_information_list(
						IoInformation::type_io
						);

			//check the value of the pins and update accordingly
			for(const auto & info: io_information_list){

				PinMarkerBar * pin_marker_bar =
						find<PinMarkerBar>("PinMarkerBar");

				if( pin_marker_bar != nullptr ){
					Io io(info.io_pin());
					pin_marker_bar->set_pin_marker_elevated(
								info.io_pin(),
								io.value()
								);

					Button * button = find<Button>(
								pin_button_name(info)
								);

					if( button != nullptr ){
						pin_marker_bar->set_pin_marker_state(
									info.io_pin(), button->theme_state()
									);
					}

				} else {
					//assert
				}
			}
		}
	} else if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event =
				event.reinterpret<ButtonEvent>();
		if( event.id() == ButtonEvent::id_released ){


			application().printer().info("toggle " + button_event.name());
			IoInformation information(button_event.name());
			Io io(information);
			if( io.is_output() ){
				io << !io.value();
			}

		} else if( event.id() == ButtonEvent::id_held ){

			event_loop()
					->layout()
					->find<PinConfiguration>("PinConfiguration")
					->set_io_pin(
						IoInformation(button_event.name()).io_pin()
						);

			event_loop()->layout()->transition("PinConfiguration");
		}
	}
}



