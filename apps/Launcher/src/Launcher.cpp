#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <sapi/fs.hpp>
#include <ToolboxAPI/components.hpp>

#include "Launcher.hpp"
#include "Application.hpp"

Launcher::Launcher(Application* application)
	: toolbox::ApplicationLayout<Launcher>("Launcher", application){

	const String launcher_file_path = "/home/Launcher.json";

	JsonObject launcher_object =
			JsonDocument().load(
				JsonDocument::FilePath(launcher_file_path)
				);

	add_component(
				TopNavigation::create(
					"TopNavigation",
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_title("Launcher"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	List * list = &List::create("List", event_loop());

	list->set_item_height(250)
			.set_vertical_scroll_enabled();

	if( launcher_object.is_valid() ){
		var::Vector<String> keys = launcher_object.keys();
		for(const auto & key: keys){
			JsonObject item_object = launcher_object.at(key).to_object();
			String name = item_object.at("name").to_string();
			list->add_component(
						ListItem::create(key)
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
	set_event_handler(event_handler);

}

void Launcher::local_event_handler(const Event & event){

	ListItem * list_item = ListEvent::component(event);
	if( list_item ){
		JsonObject launcher_object =
				JsonDocument().load(
					fs::File::Path("/home/Launcher.json")
					).to_object();

		JsonObject item_object =
				launcher_object.at(list_item->name()).to_object();

		if( item_object.is_valid() ){
			String path = item_object.at("path").to_string();
			application()->launch(path);
		}
		return;
	}

	Button * button = ButtonEvent::component(event,  ButtonEvent::id_released);
	if( button && button->name() ==
			find<TopNavigation>("TopNavigation")->left_button_name()
			){
		application()->go_home();
	}
}
