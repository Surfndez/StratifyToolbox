#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;


class Control : public toolbox::ApplicationLayout<Application> {
public:
	 Control(Application & application);

private:

	 var::Array<s8, toolbox::Io::last_io_pin> m_pin_states;

};

#endif // CONTROL_HPP
