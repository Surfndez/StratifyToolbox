
#include <sapi/ux.hpp>

#include "Application.hpp"
#include "Launcher.hpp"

Application::Application(const sys::Cli & cli)
	: toolbox::ToolboxApplication(cli){}

ux::Layout & Application::create_layout(){

	return Layout::create("Application", event_loop())
			.add_component(
				Launcher::create(this)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.set_drawing_area(
				DrawingArea(1000,1000)
				);

}

