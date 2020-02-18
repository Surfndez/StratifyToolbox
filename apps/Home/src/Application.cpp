
#include <sapi/ux.hpp>
#include <sapi/var.hpp>

#include "Application.hpp"

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

	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

		if( (button_event.id() == ButtonEvent::id_released) &&
				(button_event.name() == "SettingsButton") ){
			application.printer().info("launch settings");
			//Application::launch("/home/Settings", String() );
		} else if( (button_event.id() == ButtonEvent::id_released) &&
							 (button_event.name() == "LaunchButton") ){
			application.printer().info("launch Launcher");
			//Application::launch("/home/Launcher", String() );
		} else if( (button_event.id() == ButtonEvent::id_released) &&
							 (button_event.name() == "WifiButton") ){
			printer().info("launch Wifi Settings");
		} else if( (button_event.id() == ButtonEvent::id_released) &&
							(button_event.name() == "EjectButton") ){
		 printer().info("Eject SD Card");
	 }

	}

}
