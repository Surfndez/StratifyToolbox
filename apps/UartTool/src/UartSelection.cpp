#include <sapi/var.hpp>
#include "Application.hpp"
#include "UartSelection.hpp"
#include "Menu.hpp"

UartSelection::UartSelection(Application * app)
	: ApplicationLayout("UartSelection", app){


	const StringList uart_selection_options = {
		"A", "B", "C", "D"
	};

	constexpr drawing_size_t list_height = 500;

	add_component(
				Menu::create_options_list(
					"Uart",
					event_loop(),
					MenuListOptions()
					.set_active("A")
					.set_list(uart_selection_options)
					.set_list_height(list_height)
					.set_list_item_height(300) //show 3 items
					)

				.set_caller(name())
				);

	//menu is TopNavigation::height() + 500
	constexpr drawing_size_t menu_height = TopNavigation::height() + list_height;
	constexpr drawing_int_t legend_y = menu_height;
	constexpr drawing_size_t legend_height = 250;
	constexpr drawing_int_t pin_marker_y = legend_y + legend_height;
	constexpr drawing_size_t pin_marker_height = 1000 - (pin_marker_y);

	add_component(
				Label::create("TX")
				.set_horizontal_margin(50)
				.set_vertical_margin(10)
				.set_drawing_point(0,legend_y)
				.set_drawing_area(500,legend_height)
				.set_theme_style(Theme::style_success)
				);

	add_component(
				Label::create("RX")
				.set_horizontal_margin(50)
				.set_vertical_margin(10)
				.set_drawing_point(500,legend_y)
				.set_drawing_area(500,legend_height)
				.set_theme_style(Theme::style_warning)
				);

	add_component(
				PinMarkerBar::create(
					"PinMarkerBarName",
					event_loop(),
					IoInfo::type_io
					)
				//bottom 10% of screen (full width)
				.set_drawing_point(DrawingPoint(0,175+750))
				.set_drawing_area(DrawingArea(1000,1000-175-750))
				);

	PinMarkerBar * pin_marker_bar = find<PinMarkerBar>("PinMarkerBarName");

	for(u32 i= Io::first_io_pin;
			i < Io::last_io_pin;
			i++){
		enum Io::io_pins io_pin = static_cast<enum Io::io_pins>(i);
		IoPinDescription pin_description(io_pin);

		pin_marker_bar->set_pin_marker_enabled(
					io_pin,
					pin_description.is_peripheral_function_supported(Io::peripheral_function_uart)
					);
	}

}

void UartSelection::local_event_handler(const ux::Event & event){

	Button * button = ButtonEvent::component(event);
	if( button ){

		if( event.id() == ButtonEvent::id_released ){
			printer().info("Handle Uart Selection Menu button press: "	+ button->name() + " for " + name());

			if( button->name() == "UartTopNavigationLeftButton" ){
				parent()->transition(caller());
			}

		} else if( event.id() == ButtonEvent::id_held ){

		}

	}
}
