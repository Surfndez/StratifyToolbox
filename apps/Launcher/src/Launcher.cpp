#include <sapi/ux.hpp>
#include <sapi/var.hpp>

#include "Launcher.hpp"
#include "Application.hpp"

Launcher::Launcher(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);

	List * list = new List();

	JsonObject launcher_object =
			JsonDocument().load(
				fs::File::Path("/home/Launcher.json")
				).to_object();

	if( launcher_object.is_valid() ){

		var::Vector<String> keys = launcher_object.keys();
		for(const auto & key: keys){
			JsonObject item_object = launcher_object.at(key).to_object();
			list->append(ListItem()
									 .set_key(item_object.at("name").to_string())
									 .set_icon("chevron-right")
									 );
		}

		list->set_item_height(200)
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(1000,1000))
				.set_event_handler(
					[&](Component * object, const Event & event){

			if( event.type() == ListEvent::event_type() ){
				const ListItem & item = event.reinterpret<ListEvent>().item();

				printer().info("Launch %s", item.key().cstring());
			}
		});


		add_component(
					"List",
					*list
					);

	}



#if 0
	add_component(
				"List",
				(* new List())
				.append(ListItem()
								.set_key("Account")
								.set_value("tgil")
								.set_icon("chevron-right")
								)
				.append(ListItem()
								.set_key("Display")
								.set_icon("chevron-right")
								)
				.append(ListItem()
								.set_key("About")
								.set_icon("chevron-right")
								)
				.append(ListItem()
								.set_key("Exit")
								.set_icon("ellipsis-v")
								)
				.set_item_height(250)
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(1000,1000))
				.set_theme_style(Theme::style_brand_primary)
				.set_event_handler(
					[&](Component * object, const Event & event){

					if( event.type() == ListEvent::event_type() ){
						const ListItem & item = event.reinterpret<ListEvent>().item();

						if( item.key() == "Display" ){
							application.printer().info("set scene collection to Display");
							scene_collection()->set_current_scene("Display");
						} else if( item.key() == "About" ){
							application.printer().info("set scene collection to About");
							scene_collection()->set_current_scene("About");
						} else if( item.key() == "Exit" ){
							Application::launch("/home/Home");
						}


					}

				})

			);


#endif

}
