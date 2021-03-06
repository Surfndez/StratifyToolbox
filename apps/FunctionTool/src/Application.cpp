
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "Configuration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli)
{

}

ux::Layout & Application::create_layout(){

	return Layout::create("Application", event_loop())
			.add_component(
				Control::create(this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				Configuration::create(this)
				.set_enabled(false)
				)
			.add_component(
				About::create(this)
				.set_enabled(false)
				)
			.add_component(
				Keyboard::create("Keyboard", event_loop())
				.set_enabled(false)
				);

}



