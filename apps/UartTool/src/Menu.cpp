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
		ux::EventLoop* event_loop
		) : LayoutAccess<Menu>(name,event_loop){

	constexpr drawing_size_t top_navigation_height = 175;

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
				.set_item_height(250)
				.set_vertical_scroll_enabled()
				.set_drawing_point(DrawingPoint(0,TopNavigation::height()))
				.set_drawing_area(DrawingArea(1000,1000-TopNavigation::height()))
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
			printf("item %s\n", menu_item->name().cstring());
			switch(menu_item->type()){
				case MenuItem::type_bool:
					menu_item->set_checked( !menu_item->is_checked() );
					menu_item->redraw();
					break;
				case MenuItem::type_menu:
					printf("menu transition to %s\n", menu_item->target_name().cstring());
					parent()->transition(menu_item->key());
					break;
				case MenuItem::type_string_list:
				{
					//set string list menu parameters
					const String screen_name = menu_item->name().split("@").at(0);
					parent()->transition(screen_name);
				}
					break;
				case MenuItem::type_string:
				{
					String menu_name = menu_item->name() + "Menu";
					printf("transition to edit string %s\n", menu_name.cstring());
					Component * next_menu = parent()->find<Component>(menu_name);
					if( next_menu == nullptr ){
						printf("menu %s not found\n", menu_name.cstring());
					} else {
						printf("new menu is %p (%d)\n", next_menu, next_menu->is_layout());
					}
				}
					break;
				case MenuItem::type_radio:
					//set present value to false for all other items in the list
					const String screen_name = menu_item->name().split("@").at(0);
					if( screen_name == name() ){
						const String menu_name = screen_name + "Menu";
						List * list = find<List>(menu_name);

						for(LayoutComponent& component: list->component_list()){
							MenuItem * item = component.component()->reinterpret<MenuItem>();
							item->set_checked(
										item->key() == menu_item->key()
										);
							if( item->key() == menu_item->key() ){
								find_caller_menu_item(name())->set_present_value(item->key());
							}
							item->redraw();
						}
					}
					break;
			}
		}
	}
}






