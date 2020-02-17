
#include <sapi/ux.hpp>

#include "Application.hpp"
#include "Home.hpp"
#include "Configuration.hpp"
#include "PinConfiguration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
	: toolbox::Application(cli)
{

}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout(event_loop())))
			.add_component(
				"Home",
				(* new Home(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				"Control",
				(* new Control(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.add_component(
				"Configuration",
				(* new Configuration(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
			#if 0
				.add_component(
					"PinConfiguration",
					(* new PinConfiguration(*this))
					.set_drawing_point(
						DrawingPoint(0,0)
						)
					.set_drawing_area(
						DrawingArea(1000,1000)
						)
					)
			#endif
				)
			.set_event_handler(handle_application_event)
			.set_drawing_area(
				DrawingArea(1000,1000)
				);

}


void Application::handle_application_event(
		ux::Component * object,
		const ux::Event & event
		){

	Layout * layout = static_cast<Layout*>(object);
	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

		if( button_event.id() == ButtonEvent::id_released ){
			printf("Pushed button %s\n", button_event.name().cstring());
			if( button_event.name() == "ConfigurationButton" ){
				layout->transition("Configuration");
			} else if( button_event.name() == "ControlButton" ){
				layout->transition("Control");
			} else if( button_event.name() == "AboutButton" ){
				//layout->transition("Home");
			} else if( button_event.name() == "ExitButton" ){
				//launch the home applicatione
				go_home();
			} else if( button_event.name() == "BackHome" ){
				layout->transition("Home");
			}
		}

	}

}


