
#include <sapi/ux/Component.hpp>

#include "Application.hpp"
#include "Home.hpp"
#include "Configuration.hpp"
#include "PinConfiguration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli)
{
   scene_collection().add_scene(
						"Home",
            new Home(*this)
            );

	 scene_collection().add_scene(
						"Control",
						new Control(*this)
						);

	 scene_collection().add_scene(
						"Configuration",
						new Configuration(*this)
						);

	 scene_collection().add_scene(
						"PinConfiguration",
						new PinConfiguration(*this)
						);

	 scene_collection().add_scene(
						"About",
						new Configuration(*this)
						);
}

