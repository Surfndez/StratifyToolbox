#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;
class About : public ApplicationLayout<About> {
public:
	About(Application* application);


	static const char * top_navigation_name(){
		return "AboutTopNavigation";
	}

	void local_event_handler(const Event & event);

private:



};

#endif // ABOUT_HPP
