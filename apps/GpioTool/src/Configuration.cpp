#include "Configuration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal.hpp>
#include <sapi/ux.hpp>

Configuration::Configuration(Application & application)
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
		enum Theme::style style = Theme::style_brand_primary;

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
								application.printer().info(
								"button %s %s",
								object->name().cstring(),
								button_event.name().cstring()
								);

								scene_collection()->set_current_scene("PinConfiguration");


							}

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



