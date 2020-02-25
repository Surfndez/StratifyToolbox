#include "Control.hpp"
#include "Application.hpp"
#include "Configuration.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>


Control::Control(Application & app)
	: ApplicationLayout<Application>("Control", app){

	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("Function Tool"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	const DrawingArea button_area(400, 250);
	const DrawingArea label_area(400, 150);
	constexpr drawing_int_t button_y = 300;
	constexpr u8 button_vertical_padding = 25;
	constexpr enum Theme::style output0_theme_style = Theme::style_brand_secondary;
	constexpr enum Theme::style output1_theme_style = Theme::style_info;

	add_component(
				Component::create<Button>(
					"output0"
					)
				.set_label("Output 0")
				.set_theme_style(output0_theme_style)
				.set_vertical_padding(button_vertical_padding)
				.set_drawing_point(
					DrawingPoint(500/2 - button_area.width()/2, button_y)
					)
				.set_drawing_area(button_area)
				);

	add_component(
				Component::create<Label>(
					"status0"
					)
				.set_label("off")
				.set_theme_style(Theme::style_outline_dark)
				.set_vertical_padding(button_vertical_padding)
				.set_border_size(0)
				.set_drawing_point(
					DrawingPoint(500/2 - button_area.width()/2, button_y + button_area.height() + 50)
					)
				.set_drawing_area(label_area)
				);

	add_component(
				Component::create<Button>(
					"output1"
					)
				.set_theme_style(output1_theme_style)
				.set_label("Output 1")
				.set_vertical_padding(button_vertical_padding)
				.set_drawing_point(
					DrawingPoint(500 + 500/2 - button_area.width()/2, button_y)
					)
				.set_drawing_area(button_area)
				);

	add_component(
				Component::create<Label>(
					"status1"
					)
				.set_label("off")
				.set_theme_style(Theme::style_outline_dark)
				.set_vertical_padding(button_vertical_padding)
				.set_border_size(0)
				.set_drawing_point(
					DrawingPoint(500+500/2 - button_area.width()/2, button_y + button_area.height() + 50)
					)
				.set_drawing_area(label_area)
				);

	add_component(
				Component::create<PinMarkerBar>(
					pin_marker_bar_name(),
					event_loop(),
					IoInfo::type_analog
					)
				//disable ADC pins
				.set_pin_marker_enabled(Io::io_pin16, false)
				.set_pin_marker_enabled(Io::io_pin17, false)
				//match style of buttons
				.set_pin_marker_style(Io::io_pin18, output0_theme_style)
				.set_pin_marker_style(Io::io_pin19, output1_theme_style)
				//bottom 10% of screen (full width)
				.set_drawing_point(DrawingPoint(0,900))
				.set_drawing_area(DrawingArea(1000,100))
				);



	set_event_handler(Control::event_handler);

}

void Control::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){
			update_pin_markers();
		}
	} else if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event =
				event.reinterpret<ButtonEvent>();

		if( event.id() == ButtonEvent::id_held ){
			if( button_event.name() == "output0" ){
				event_loop()->layout()->find<Configuration>("Configuration")
						->set_io_pin(
							Io::io_pin18
							);
				event_loop()->layout()->transition("Configuration");
			} else if( button_event.name() == "output1" ){
				event_loop()->layout()->find<Configuration>("Configuration")
						->set_io_pin(
							Io::io_pin19
							);
				event_loop()->layout()->transition("Configuration");
			}



		} else if( event.id() == ButtonEvent::id_released ){

			if( button_event.name() ==
					find<TopNavigation>(top_navigation_name())->right_button_name()
					){
				event_loop()->layout()->transition("About");
			} else if( button_event.name() ==
											find<TopNavigation>(top_navigation_name())->left_button_name()
											){
				application().go_home();
			}

		}
	}
}

bool Control::handle_io_button_press(const ux::ButtonEvent& button_event){
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


void Control::update_pin_markers(){


}





