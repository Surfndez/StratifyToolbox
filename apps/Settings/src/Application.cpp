
#include <sapi/var.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "Settings.hpp"
#include "Display.hpp"
#include "About.hpp"

Application::Application(const sys::Cli & cli)
	 : toolbox::ToolboxApplication(cli){


}

ux::Layout & Application::create_layout(){

	return Layout::create("Application", event_loop())
			.add_component(
				Settings::create(this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				Display::create(this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
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



