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

	MenuItem(const var::String& name) : ux::ListItemAccess<MenuItem>(name){
		set_key(name);
	}

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

	MenuItem& set_present_value(const var::String& value){
		m_present_value = value;
		update_value();
		return *this;
	}

	bool is_checked() const {
		return present_value() == "true";
	}


private:
	API_ACCESS_COMPOUND(MenuItem,var::String,target_name);
	API_READ_ACCESS_FUNDAMENTAL(MenuItem,enum types,type,type_bool);
	API_READ_ACCESS_COMPOUND(MenuItem,var::String,present_value);

	void update_value(){
		switch(type()){
			case type_radio:
			case type_bool:
				present_value() == "true" ?
							set_value("icon@check") :
							set_value("");
				break;
			case type_string:
			case type_string_list:
				set_value(present_value());
				break;
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

	var::String get_top_navigation_name() const {
		return name() + "TopNavigation";
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

	Menu& add_filler(){
		m_list->add_filler();
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
	API_ACCESS_COMPOUND(Menu,var::String,caller);
	API_ACCESS_COMPOUND(Menu,var::String,menu_item);

	static void event_handler(
			ux::Layout * object,
			const ux::Event & event
			){
		object->reinterpret<Menu>()->local_event_handler(event);
	}

	void local_event_handler(const ux::Event & event);

	MenuItem * find_caller_menu_item(const var::String& name){
			return parent()->find<Menu>(caller())
			->find<ux::List>(caller() + "Menu")
			->find<MenuItem>(name);
	}

};

#endif // MENU_HPP
