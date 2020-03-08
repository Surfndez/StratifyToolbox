
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

	return Layout::create("Application", event_loop())
			.add_component(
				Keyboard::create(this)
				);

}



