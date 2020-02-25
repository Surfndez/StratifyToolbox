#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Control : public ApplicationLayout<Application> {
public:
	 Control(Application & app);

	 static const char * top_navigation_name(){
		 return "ControlTopNavigation";
	 }

	 static const char * pin_marker_bar_name(){
		 return "PinMarkerBar";
	 }

private:

	 var::Array<s8, Io::last_io_pin> m_pin_states;
	 static void event_handler(
			 ux::Layout * object,
			 const ux::Event & event
			 ){
		 Control * control_object = object->reinterpret<Control>();
		 control_object->local_event_handler(event);
	 }
	 void local_event_handler(const ux::Event & event);
	 bool handle_io_button_press(const ux::ButtonEvent & button_event);
	 void update_pin_markers();
};

#endif // CONTROL_HPP