#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;

class Launcher : public toolbox::ApplicationLayout<Application> {
public:
	Launcher(Application & application);
};

#endif // LAUNCHER_HPP
