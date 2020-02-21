#ifndef PINCONFIGURATION_HPP
#define PINCONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class PinConfiguration : public ApplicationLayout<Application> {
public:
	 PinConfiguration(Application & application);

	 void set_io_pin(enum Io::io_pins pin){
		 m_io_pin = pin;
	 }

private:
	 enum Io::io_pins m_io_pin;

	 void update_display_values();
};

#endif // CONFIGURATION_HPP
