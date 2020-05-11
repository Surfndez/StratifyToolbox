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

	String list_name = name + "Menu";

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("chevron-left")
					.set_right_icon_name("info")
					.set_title(name),
					event_loop)
				.set_drawing_area(DrawingArea(1000,top_navigation_height))
				);

	add_component(List::create(list_name, event_loop)
								.set_item_height(250)
								.set_vertical_scroll_enabled()
								.set_drawing_point(DrawingPoint(0,TopNavigation::height()))
								.set_drawing_area(DrawingArea(1000,1000-TopNavigation::height()))
								);

	m_list = find<ux::List>(list_name);

	set_event_handler(event_handler);

#if 0
	add_component(
				List::create("List", event_loop())
				.set_item_height(250)
				.set_vertical_scroll_enabled()
				.add_component(
					ListItem::create("Serial")
					.set_key("Serial Settings")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("Wifi")
					.set_key("Wifi")
					.set_value("icon@check")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("USB")
					.set_key("USB")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("SDCard")
					.set_key("SD Card")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.add_component(
					ListItem::create("Help")
					.set_key("Help")
					.set_value("icon@chevron-right")
					.set_vertical_padding(40)
					.set_theme_style(Theme::style_outline_dark)
					)
				.set_drawing_point(DrawingPoint(0,175))
				.set_drawing_area(DrawingArea(1000,1000-175))
				);
#endif

}

void Menu::local_event_handler(
		const ux::Event & event
		){

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
					event_loop()->layout()->transition(menu_item->target_name());
					break;
				case MenuItem::type_string_list:
				{
					//set string list menu parameters
					const String screen_name = menu_item->name().split("@").at(0);
					transition(screen_name);
				}
					break;
				case MenuItem::type_string:
				{
					String menu_name = menu_item->name() + "Menu";
					printf("transition to edit string %s\n", menu_name.cstring());
					Component * next_menu = event_loop()->layout()->find<Component>(menu_name);
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
							item->redraw();
						}
					}
					break;
			}
		}

	}

}






