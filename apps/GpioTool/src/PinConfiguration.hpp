#ifndef PINCONFIGURATION_HPP
#define PINCONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class PinConfiguration : public ApplicationLayout<Application> {
public:
	 PinConfiguration(Application & application);

private:

	 void update_display_values();
	 void toggle_direction();
};

#endif // CONFIGURATION_HPP
