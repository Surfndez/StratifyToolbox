#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;


class Control : public toolbox::ApplicationScene<Application> {
public:
	 Control(Application & application);

private:


};

#endif // CONTROL_HPP
