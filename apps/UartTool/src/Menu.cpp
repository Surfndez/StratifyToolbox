#include "Menu.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

Menu::Menu(
		const var::String& name,
		ux::EventLoop* event_loop,
		const MenuOptions& options
		) : LayoutAccess<Menu>(name,event_loop){

	constexpr drawing_size_t top_navigation_height = TopNavigation::height();

	const String list_name = name + "Menu";
	add_component(
				TopNavigation::create(
					get_top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("chevron-left")
					.set_right_icon_name("info")
					.set_title(name),
					event_loop)
				.set_drawing_area(DrawingArea(1000,top_navigation_height))
				);

	add_component(
				List::create(list_name, event_loop)
				.set_item_height(options.list_item_height())
				.set_vertical_scroll_enabled()
				.set_drawing_point(DrawingPoint(0,top_navigation_height))
				.set_drawing_area(
					DrawingArea(
						1000,
						options.list_height())
					)
				);

	m_list = find<ux::List>(list_name);

	set_event_handler(event_handler);
}

void Menu::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

		if( button_event.id() == ButtonEvent::id_released ){
			Button * button = button_event.component(event);
			if( button->name() == String(get_top_navigation_name()) + "LeftButton" ){
				parent()->transition(caller());
			}
		}
	}

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_enter ){
			//update values with child values

		}
	}

	if( event.type() == ListEvent::event_type() ){

		MenuItem * menu_item = find<MenuItem>(event.component()->name());
		if( menu_item != nullptr ){
			switch(menu_item->type()){
				case MenuItem::type_bool:
					menu_item->set_checked( !menu_item->is_checked() );
					menu_item->redraw();
					break;
				case MenuItem::type_string:
				case MenuItem::type_menu:
					printf("menu transition to '%s'\n", menu_item->target_name().cstring());
					parent()->transition(menu_item->target_name());
					break;
				case MenuItem::type_string_list:
				{
					//set string list menu parameters
					const String screen_name = menu_item->name().split("@").at(0);
					parent()->transition(screen_name);
				}
					break;
				case MenuItem::type_radio:
					//set present value to false for all other items in the list
					const String screen_name = menu_item->name().split("@").at(0);
					if( screen_name == name() ){
						const String menu_name = screen_name + "Menu";
						List * list = find<List>(menu_name);

						if( list == nullptr ){
							break;
						}


						for(LayoutComponent& component: list->component_list()){

							if( component.component()->name().find("Filler") == String::npos ){

								MenuItem * item = component.component()->reinterpret<MenuItem>();
								if( item == nullptr ){

								} else {
									item->set_checked(
												item->key() == menu_item->key()
												);
									if( item->key() == menu_item->key() ){
										MenuItem * caller_item = find_caller_menu_item(name());
										if( caller_item == nullptr ){

										} else {
											caller_item->set_present_value(item->key());
										}
									}
									item->redraw();
								}
							}
						}
					}
					break;
			}
		}
	}
}






