#include "Configuration.hpp"
#include "Application.hpp"


#include <ToolboxAPI/components/TopNavigation.hpp>
#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal.hpp>
#include <sapi/ux.hpp>

Configuration::Configuration(Application & application)
	: toolbox::ApplicationLayout<Application>(application){


	add_component(
				"ConfigurationTopNavigation",
				(* new toolbox::TopNavigation("Configuration", "BackHome", event_loop()))
				.set_drawing_area(DrawingArea(1000,175))
				);

	const u32 columns = 4;

	var::Vector<toolbox::IoInformation> io_information_list =
			toolbox::Io::io_information_list(
				toolbox::IoInformation::type_io
				);

	u32 row = 1;
	u32 column = 0;

	const DrawingArea button_area(225,200);
	const DrawingArea pin_area(45,40);

	for(const auto & info: io_information_list){
		enum Theme::style style = Theme::style_outline_dark;

		add_component(
					"Configure-" + info.name(),
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


		column++;
		if( column == columns ){
			row++;
			column = 0;
		}
	}



}



