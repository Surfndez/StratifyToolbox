#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Launcher.hpp"
#include "Application.hpp"

Launcher::Launcher(Application & application)
	: toolbox::ApplicationLayout<Application>("Launcher", application){

	add_component(
				Component::create<TopNavigation>(
					"TopNavigation",
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_title("Launcher"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				Component::create<TopNavigation>(
					"TopNavigation",
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_title("Launcher"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	List * list = new List("List", event_loop());
	list->set_item_height(250)
			.set_vertical_scroll_enabled();



	JsonObject launcher_object =
			JsonDocument().load(
				fs::File::Path("/home/Launcher.json")
				).to_object();

	if( launcher_object.is_valid() ){
		var::Vector<String> keys = launcher_object.keys();
		for(const auto & key: keys){
			JsonObject item_object = launcher_object.at(key).to_object();
			String name = item_object.at("name").to_string();
			list->add_component(
						Component::create<ListItem>(name)
						.set_key(name)
						.set_icon("chevron-right")
						.set_vertical_padding(40)
						.set_theme_style(Theme::style_outline_dark)
						);
		}
	}

	list->set_drawing_point(DrawingPoint(0,175))
			.set_drawing_area(DrawingArea(1000,1000-175));

	add_component(*list);

}

void Launcher::local_event_handler(const Event & event){
	printf("handle event\n");
}
