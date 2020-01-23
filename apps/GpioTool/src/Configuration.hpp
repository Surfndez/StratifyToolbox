#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;


class Configuration : public toolbox::ApplicationScene<Application> {
public:
	 Configuration(Application & application);

private:


};

#endif // CONFIGURATION_HPP
