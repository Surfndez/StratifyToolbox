
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "Keyboard.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli)
{

}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout("Application", event_loop())))
			.add_component(
				(*(new Keyboard(*this)))
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



