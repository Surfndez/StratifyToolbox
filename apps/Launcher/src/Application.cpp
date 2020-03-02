
#include <sapi/ux.hpp>

#include "Application.hpp"
#include "Launcher.hpp"

Application::Application(const sys::Cli & cli)
	 : toolbox::ToolboxApplication(cli){}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout("Application", event_loop())))
			.add_component(
				(* new Launcher(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.set_event_handler([this](ux::Component * object,
												 const ux::Event & event){
		//Application::handle_application_event(*this, object, event);
	}
	)
	.set_drawing_area(
	DrawingArea(1000,1000)
	);

}

