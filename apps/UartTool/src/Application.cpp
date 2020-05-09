
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli){

}

ux::Layout & Application::create_layout(){

	return Layout::create("Application", event_loop())
			.add_component(
				Control::create(this)
				)
			.add_component(
				About::create(this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.set_drawing_area(
				DrawingArea(1000,1000)
				);

}



