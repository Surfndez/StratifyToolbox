
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "PinConfiguration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli)
{

}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout("Application", event_loop())))
			.add_component(
				(*(new Control(*this)))
				//Component::create<Control>(*this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				(*(new PinConfiguration(*this)))
				//Component::create<PinConfiguration>(*this)
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.add_component(
				(*(new About(*this)))
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



