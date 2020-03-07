
#include <sapi/ux.hpp>
#include <sapi/var.hpp>

#include "Application.hpp"

Application::Application(const sys::Cli & cli)
	: toolbox::ToolboxApplication(cli){}


ux::Layout & Application::create_layout(){
	return (Layout&)(*(new Layout("Application", event_loop())))
			.add_component(
				(* new Home(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
	.set_drawing_area(
	DrawingArea(1000,1000)
	);

}
