
#include <sapi/var.hpp>

#include "Application.hpp"
#include "Settings.hpp"
#include "Display.hpp"
#include "About.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli){


}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout(event_loop())))
			.add_component(
				"Settings",
				(* new Settings(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				"Display",
				(* new Display(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.add_component(
				"About",
				(* new About(*this))
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

	if( event.type() == ListEvent::event_type() ){
		const ListEvent & list_event = event.reinterpret<ListEvent>();
		if( list_event.item().name() == "Display" ){
			layout->transition("Display");
		} else if( list_event.item().name() == "About" ){
			layout->transition("About");
		} else if( list_event.item().name() == "Exit" ){
			go_home();
		}

	}

	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
		if( button_event.name() == "BackHome" ){
			layout->transition("Settings");
		}

	}

}
