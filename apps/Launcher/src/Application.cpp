
#include <sapi/ux/Component.hpp>

#include "Application.hpp"
#include "Launcher.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli)
{
   scene_collection().add_scene(
						"Launcher",
						new Launcher(*this)
            );
}

