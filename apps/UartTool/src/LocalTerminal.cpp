#include "LocalTerminal.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>



LocalTerminal::LocalTerminal(Application * app)
	: ApplicationLayout("LocalTerminal", app){

	constexpr drawing_size_t top_navigation_height = TopNavigation::height();
	constexpr drawing_size_t input_text_height = 200;
	constexpr drawing_size_t local_terminal_text_height = 1000 - top_navigation_height - input_text_height;

	constexpr drawing_int_t local_terminal_text_y = top_navigation_height;
	constexpr drawing_int_t input_text_y = local_terminal_text_y + local_terminal_text_height;
	constexpr drawing_size_t input_text_width = 850;
	constexpr drawing_size_t send_button_x = input_text_width;
	constexpr drawing_size_t send_button_width = 1000-input_text_width;

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("Uart Tool"),
					event_loop())
				);

	//add the text area
	add_component(
				Label::create("LocalTerminalText")
				.set_label("Terminal")
				.set_theme_style(Theme::style_outline_dark)
				.set_drawing_point(0, local_terminal_text_y)
				.set_drawing_area(1000, local_terminal_text_height)
				);

	add_component(
				Button::create("EditButton")
				.set_drawing_point(0, input_text_y)
				.set_drawing_area(input_text_width, input_text_height)
				);

	add_component(
				Button::create("SendButton")
				.set_drawing_point(send_button_x, input_text_y)
				.set_drawing_area(send_button_width, input_text_height)
				);
}

void LocalTerminal::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){
			//update terminal with incoming UART data

		}
		return;
	}

	Button * button = ButtonEvent::component(event);
	if( button ){

		if( event.id() == ButtonEvent::id_released ){
			printer().info("Handle button press: "	+ button->name());

			//edit button -- jump to keyboard component
			if( button->name() == "EditButton" ){
				printf("Transition to edit\n");
			}

			//send button -- transmit data on UART
			if( button->name() == "SendButton" ){
				printf("transmit data on UART\n");
			}

			//settings button -- transition to settings menu
			if( button->name() == find<TopNavigation>(top_navigation_name())->right_button_name() ){
				printf("transition to settings from %p %s\n", parent(), name().cstring());
				parent()->transition("Configuration");
			}

		} else if( event.id() == ButtonEvent::id_held ){

		}

	}
}





