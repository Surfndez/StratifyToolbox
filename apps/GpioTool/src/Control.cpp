#include "Control.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

Control::Control(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);

	for(auto & value: m_pin_states){
		value = -1;
	}

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

	const u32 columns = 4;

	var::Vector<toolbox::IoInformation> io_information_list =
			toolbox::Io::io_information_list(
				toolbox::IoInformation::type_io
				);

	u32 row = 0;
	u32 column = 1;

	const DrawingArea button_area(225,200);
	const DrawingArea pin_area(45,40);

	for(const auto & info: io_information_list){
		u16 pin = info.physical_pin_number();
		enum Theme::style style =
				static_cast<Theme::style>(pin % Theme::last_style);

		add_component(
					info.name(),
					(* new ux::Button())
					.set_border_size(1)
					.set_label(info.name())
					.set_theme_style(style)
					.set_drawing_point(
						DrawingPoint(
							column*(button_area.width()+25)+25/2,
							row*(button_area.height()+25)+25
							)
						)
					.set_drawing_area(button_area)
					.set_event_handler([&](Component * object, const Event & event){


						if( event.type() == ButtonEvent::event_type() ){
							toolbox::IoInformation io_information(object->name());
							const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();


							if( button_event.id() == ButtonEvent::id_released ){
								application.printer().info("button %s %s", object->name().cstring(), button_event.name().cstring() );

								Pin pin(
								Pin::Port(0),
								Pin::PinNumber(io_information.physical_pin_number())
								);

								pin.open();
								printer().info("pin %d value is %d",
								io_information.physical_pin_number(),
								pin.get_value()
								);


							}

						}

					})
				);

		add_component(
					String().format("io%d", pin),
					(* new ux::Label())
					.set_border_size(1)
					.set_theme_style(style)
					.set_drawing_point(
						DrawingPoint(
							(pin-1)*pin_area.width()+50,
							1000-pin_area.height()
							)
						)
					.set_drawing_area(pin_area)
					.set_event_handler([pin_area, this](Component * object, const Event & event){

						if( (event.type() == SystemEvent::event_type()) &&
						(event.id() == SystemEvent::id_update) ){
							toolbox::IoInformation io_information(object->name());

							Pin pin(
							Pin::Port(0),
							Pin::PinNumber(io_information.physical_pin_number())
							);

							pin.open();

							int current_value = pin.get_value();

							if( current_value != m_pin_states.at(io_information.physical_pin_number())){

								if( pin.get_value() == true ){
									object->erase();
									object->set_drawing_point(
									DrawingPoint(
									(io_information.physical_pin_number()-1)*pin_area.width()+50,
									1000-2*pin_area.height()
									));
								} else {
									object->erase();
									object->set_drawing_point(
									DrawingPoint(
									(io_information.physical_pin_number()-1)*pin_area.width()+50,
									1000-pin_area.height()
									));
								}

								object->redraw();
							}

							m_pin_states.at(io_information.physical_pin_number()) = current_value;

						}


					})
				);

		column++;
		if( column == columns ){
			row++;
			column = 0;
		}
	}

}



