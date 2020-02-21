#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Control : public ApplicationLayout<Application> {
public:
	 Control(Application & app);

	 static const var::String pin_button_name(const IoInformation & information);
	 static const var::String pin_marker_name(const IoInformation & information);


private:

	 var::Array<s8, Io::last_io_pin> m_pin_states;
	 static void handle_event(
			 ux::Layout * object,
			 const ux::Event & event
			 ){
		 Control * control_object = object->reinterpret<Control>();
		 control_object->handle_control_event(event);
	 }


	 void handle_control_event(const ux::Event & event);
};

#endif // CONTROL_HPP
