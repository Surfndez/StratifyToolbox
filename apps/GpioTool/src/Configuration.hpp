#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;


class Configuration : public ApplicationLayout<Application> {
public:
	 Configuration(Application & application);

	 static const var::String pin_button_name(
			 const IoInformation& information
			 );
private:


};

#endif // CONFIGURATION_HPP
