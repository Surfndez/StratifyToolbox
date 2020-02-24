#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Keyboard : public ApplicationLayout<Application> {
public:
	 Keyboard(Application & app);


private:

	 var::String m_text;
	 var::String m_display_text;
	 u32 m_button_set;

	 static constexpr u32 key_count(){ return 26; }

	 static const var::String top_navigation_name(){
		 return "TopNav";
	 }

	 var::Array<var::String, 4> m_button_options;


	 const var::String & button_set() const {
		 return m_button_options.at(m_button_set);
	 }
	 static void event_handler(
			 ux::Layout * object,
			 const ux::Event & event
			 ){
		 Keyboard * control_object = object->reinterpret<Keyboard>();
		 control_object->local_event_handler(event);
	 }
	 void local_event_handler(const ux::Event & event);
	 void update_symbols();
	 void update_text();
};

#endif // CONTROL_HPP
