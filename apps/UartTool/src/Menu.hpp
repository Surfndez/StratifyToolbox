#ifndef MENU_HPP
#define MENU_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;

class MenuItem : public ux::ListItemAccess<MenuItem> {
public:

	enum types {
		type_bool,
		type_string,
		type_menu,
		type_string_list,
		type_radio
	};

	MenuItem(const var::String& name) : ux::ListItemAccess<MenuItem>(name){}

	MenuItem& set_type(enum types value){
		m_type = value;
		update_value();
		return *this;
	}

	MenuItem& set_checked(bool value=true){
		set_present_value(value ? "true" : "false");
		update_value();
		return *this;
	}

	bool is_checked() const {
		return present_value() == "true";
	}


private:
	API_ACCESS_COMPOUND(MenuItem,var::String,target_name);
	API_READ_ACCESS_FUNDAMENTAL(MenuItem,enum types,type,type_bool);
	API_ACCESS_COMPOUND(MenuItem,var::String,present_value);

	void update_value(){
		switch(type()){
			case type_radio:
			case type_bool:
				present_value() == "true" ?
							set_value("icon@check") :
							set_value("");
				break;
			case type_string:
				break;
			case type_string_list:
			case type_menu:
				set_value("icon@chevron-right");
				break;
		}
	}
};


class Menu : public ux::LayoutAccess<Menu> {
public:
	Menu(
			const var::String& name,
			ux::EventLoop* event_loop
			);

	static const char * top_navigation_name(){
		return "MenuTopNavigation";
	}

	Menu& add_item(MenuItem& item){
		item.set_vertical_padding(40)
				.set_theme_style(Theme::style_outline_dark);
		m_list->add_component(item);
		return *this;
	}

	Menu& add_menu(Menu& menu){
		this->add_component(menu);
		return *this;
	}

	static Menu& create_options_list(
			const var::String& name,
			ux::EventLoop * event_loop,
			const var::String& active,
			const var::StringList& list
			){
		Menu& result =
				Menu::create(name, event_loop);
		for(const var::String& item: list){
			result.add_item(
						MenuItem::create(name + "@" + item)
						.set_type(MenuItem::type_radio)
						.set_key(item)
						);
		}
		return result;
	}


private:
	API_READ_ACCESS_FUNDAMENTAL(Menu,ux::List*,list,nullptr);

	static void event_handler(
			ux::Layout * object,
			const ux::Event & event
			){
		object->reinterpret<Menu>()->local_event_handler(event);
	}

	void local_event_handler(const ux::Event & event);

};

#endif // MENU_HPP
