#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/ToolboxApplication.hpp>

class Application;

class Launcher : public toolbox::ApplicationLayout<Launcher> {
public:
	Launcher(Application * application);

private:

	static void event_handler(
			Component * object,
			const Event & event){
		object->reinterpret<Launcher>()->local_event_handler(event);
	}

	void local_event_handler(const Event & event);
};

#endif // LAUNCHER_HPP
