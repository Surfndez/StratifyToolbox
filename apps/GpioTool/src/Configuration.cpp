#include "Configuration.hpp"
#include "Application.hpp"


#include <ToolboxAPI/components/TopNavigation.hpp>
#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/hal.hpp>
#include <sapi/ux.hpp>

const var::String Configuration::pin_button_name(
		const IoInformation & information){
	return "Configure-" + information.name();
}


Configuration::Configuration(Application & application)
	: ApplicationLayout<Application>(application){


	add_component(
				"ConfigurationTopNavigation",
				(* new TopNavigation(
					 TopNavigationAttributes()
					 .set_title("Configuration"),
					 event_loop()))
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



