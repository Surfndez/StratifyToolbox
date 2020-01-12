
#include "Application.hpp"
#include "Settings.hpp"
#include "Display.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli){

   scene_collection().add_scene(
            "Settings",
            new Settings(*this)
            );

   scene_collection().add_scene(
            "Display",
            new Display(*this)
            );
}

