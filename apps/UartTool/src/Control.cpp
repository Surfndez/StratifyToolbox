#include "Control.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

const var::String Control::pin_button_name(
		const IoInfo & information
		){
	return information.name();
}

const var::String Control::pin_marker_name(
		const IoInfo & information){
	return PinMarkerBar::pin_marker_name(information.io_pin());
}


Control::Control(Application * app)
	: ApplicationLayout("Control", app){

	for(auto & value: m_pin_states){
		value = -1;
	}

	PRINTER_TRACE(printer(), "construct");
	constexpr drawing_size_t top_navigation_height = TopNavigation::height();
	constexpr drawing_size_t button_height = 350;
	constexpr drawing_size_t pin_marker_bar_height = 1000-175-button_height*2;
	constexpr drawing_int_t button_y_offset = top_navigation_height;
	constexpr drawing_size_t pin_marker_y_offset = button_y_offset + button_height*2;

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("Uart Tool"),
					event_loop())
				);

	add_component(
				PinMarkerBar::create(
					pin_marker_bar_name(),
					event_loop(),
					IoInfo::type_io
					)
				//bottom 10% of screen (full width)
				.set_drawing_point(DrawingPoint(0,pin_marker_y_offset))
				.set_drawing_area(DrawingArea(1000,pin_marker_bar_height))
				);

	PinMarkerBar * pin_marker_bar = find<PinMarkerBar>("PinMarkerBar");
	for(auto i: Io::all_io_pins){
		IoPinDescription pin_description(i);
		pin_marker_bar->set_pin_marker_enabled(
					i,
					pin_description
					.is_peripheral_function_supported(Io::peripheral_function_uart)
					);
	}

	if( pin_marker_bar == nullptr ){
		printer().error("pin marker bar is null");
		exit(1);
	}

	const DrawingArea button_area(1000/2,button_height);

	add_component(
				Button::create("UartA")
				.set_border_size(1)
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_horizontal_margin(10)
				.set_vertical_margin(25)
				.set_drawing_point(0,button_y_offset)
				.set_drawing_area(button_area)
				);

	add_component(
				Button::create("UartB")
				.set_border_size(1)
				.set_theme_style(Theme::style_outline_brand_secondary)
				.set_horizontal_margin(10)
				.set_vertical_margin(25)
				.set_drawing_point(button_area.width(),button_y_offset)
				.set_drawing_area(button_area)
				);

	add_component(
				Button::create("UartC")
				.set_border_size(1)
				.set_theme_style(Theme::style_outline_info)
				.set_horizontal_margin(10)
				.set_vertical_margin(25)
				.set_drawing_point(0,button_y_offset+button_area.height())
				.set_drawing_area(button_area)
				);

	add_component(
				Button::create("UartD")
				.set_border_size(1)
				.set_theme_style(Theme::style_outline_warning)
				.set_horizontal_margin(10)
				.set_vertical_margin(25)
				.set_drawing_point(button_area.width(),button_y_offset+button_area.height())
				.set_drawing_area(button_area)
				);

#if 0
	pin_marker_bar->set_pin_marker_style(
				info.io_pin(),
				style
				);
#endif


}

void Control::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){
			update_pin_markers();
		}
		return;
	}

	Button * button = ButtonEvent::component(event);
	if( button ){
		if( event.id() == ButtonEvent::id_released ){
			printer().info("Handle button "	+ button->name());
			if( handle_io_button_press(button) == false ){
				if( button->name() ==
						find<TopNavigation>(top_navigation_name())->left_button_name()
						){
					//exit the application and launch the home screen
					application()->go_home();
				} else if( button->name() ==
									 find<TopNavigation>(top_navigation_name())->right_button_name()
									 ){
					event_loop()->layout()->transition("About");
				}
			}
		} else if( event.id() == ButtonEvent::id_held ){

		}
	}
}

bool Control::handle_io_button_press(const ux::Button* button){
	IoInfo information(button->name());
	if( information.is_valid() ){
		Io io(information);
		if( io.is_output() ){
			io << !io.value();
		}
		return true;
	}
	return false;
}


void Control::update_pin_markers(){
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





