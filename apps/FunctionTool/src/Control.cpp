#include "Control.hpp"
#include "Application.hpp"
#include "Configuration.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

const var::String Keyboard::pin_button_name(
		const IoInfo & information
		){
	return information.name();
}

const var::String Keyboard::pin_marker_name(
		const IoInfo & information){
	return PinMarkerBar::pin_marker_name(information.io_pin());
}


Keyboard::Keyboard(Application & app)
	: ApplicationLayout<Application>("Control", app){

	for(auto & value: m_pin_states){
		value = -1;
	}

	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("GPIO Tool"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);


	const u32 columns = 4;

	var::Vector<IoInfo> io_information_list =
			Io::io_information_list(
				IoInfo::type_io
				);

	u32 row = 1;
	u32 column = 0;

	const DrawingArea button_area(225,200);
	const DrawingArea pin_area(45,40);

	add_component(
				Component::create<PinMarkerBar>(
					pin_marker_bar_name(),
					event_loop(),
					IoInfo::type_io
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
					Component::create<ux::Button>(
						pin_button_name(info)
						)
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

	set_event_handler(Keyboard::event_handler);

}

void Keyboard::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){
			update_pin_markers();
		}
	} else if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event =
				event.reinterpret<ButtonEvent>();
		if( event.id() == ButtonEvent::id_released ){

			application().printer().info("Handle button "	+ button_event.name());
			if( handle_io_button_press(button_event) == false ){
				if( button_event.name() ==
						find<TopNavigation>(top_navigation_name())->left_button_name()
						){
					//exit the application and launch the home screen
					application().go_home();
				} else if( button_event.name() ==
									 find<TopNavigation>(top_navigation_name())->right_button_name()
									 ){
					event_loop()->layout()->transition("About");
				}
			}


		} else if( event.id() == ButtonEvent::id_held ){
			event_loop()
					->layout()
					->find<PinConfiguration>("PinConfiguration")
					->set_io_pin(
						IoInfo(button_event.name()).io_pin()
						);
			event_loop()->layout()->transition("PinConfiguration");
		}
	}
}

bool Keyboard::handle_io_button_press(const ux::ButtonEvent& button_event){
	IoInfo information(button_event.name());
	if( information.is_valid() ){
		Io io(information);
		if( io.is_output() ){
			io << !io.value();
		}
		return true;
	}
	return false;
}


void Keyboard::update_pin_markers(){
	var::Vector<IoInfo> io_information_list =
			Io::io_information_list(
				IoInfo::type_io
				);

	//check the value of the pins and update accordingly
	for(const auto & info: io_information_list){

		PinMarkerBar * pin_marker_bar =
				find<PinMarkerBar>(pin_marker_bar_name());

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





