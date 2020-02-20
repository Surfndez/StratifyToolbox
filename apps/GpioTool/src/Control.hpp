#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Control : public ApplicationLayout<Application> {
public:
	 Control(Application & application);

private:

	 var::Array<s8, Io::last_io_pin> m_pin_states;
	 static void handle_event(
			 ux::Component * object,
			 const ux::Event & event
			 );
};

#endif // CONTROL_HPP
