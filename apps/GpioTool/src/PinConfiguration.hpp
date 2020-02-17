#ifndef PINCONFIGURATION_HPP
#define PINCONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>
#include <ToolboxAPI/Io.hpp>

class Application;


class PinConfiguration : public toolbox::ApplicationLayout<Application> {
public:
	 PinConfiguration(Application & application);

private:

};

#endif // CONFIGURATION_HPP