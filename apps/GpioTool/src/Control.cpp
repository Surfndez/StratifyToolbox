#include "Control.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

Control::Control(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	for(auto & value: m_pin_states){
		value = -1;
	}

	add_component(
				"BackHome",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("chevron-left")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(0, 0))
				.set_drawing_area(DrawingArea(200, 200))
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
				);

		column++;
		if( column == columns ){
			row++;
			column = 0;
		}
	}

}



