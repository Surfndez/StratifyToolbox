
#include <sapi/ux.hpp>

#include "Application.hpp"
#include "Home.hpp"
#include "Configuration.hpp"
#include "PinConfiguration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
   : toolbox::Application(cli)
{
#if 0
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
#endif
}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout()))
			.add_component(
				"Home",
				(* new Home(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)

				)
			.set_drawing_area(
				DrawingArea(1000,1000)
				);

}


