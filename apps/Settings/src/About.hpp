#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/ToolboxApplication.hpp>

class Application;

class About : public toolbox::ApplicationLayout<About> {
public:
	 About(Application * application);

	 static const char * top_navigation_name(){
		 return "AboutTopNavigation";
	 }

private:

	 static void event_handler(
			 Component * object,
			 const Event & event){
		 object->reinterpret<About>()->local_event_handler(event);
	 }

	 void local_event_handler(const Event & event);

};

#endif // ABOUT_HPP
