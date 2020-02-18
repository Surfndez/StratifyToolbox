
#include <sapi/ux.hpp>
#include <sapi/var.hpp>

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
				)
			.add_component(
				"PinConfiguration",
				(* new PinConfiguration(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.set_event_handler([this](ux::Component * object,
												 const ux::Event & event){
		Application::handle_application_event(*this, object, event);
	}
			)
			.set_drawing_area(
				DrawingArea(1000,1000)
				);

}


void Application::handle_application_event(
		Application & application,
		ux::Component * object,
		const ux::Event & event
		){

	Layout * layout = static_cast<Layout*>(object);
	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

		if( button_event.id() == ButtonEvent::id_released ){
			printf("Pushed button %s\n", button_event.name().cstring());
			if( (button_event.name() == "ConfigurationButton") ||
					(button_event.name() == "BackConfiguration") ){
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
			} else if( button_event.name().find("Configure") == 0 ){
				//update the data model with the currently selected pin

				model().insert(
							"pinConfiguration",
							JsonString(button_event.name())
							);

				layout->transition("PinConfiguration");

			}
		}

	}

}


