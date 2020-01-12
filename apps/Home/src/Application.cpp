
#include <sapi/ux/Component.hpp>

#include "Application.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli)
{
   scene_collection().add_scene(
            "home",
            new Home(*this)
            );
}

