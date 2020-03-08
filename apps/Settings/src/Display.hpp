#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/ToolboxApplication.hpp>

class Application;


class Display : public toolbox::ApplicationLayout<Display> {
public:
	 Display(Application * application);

	 static const char * top_navigation_name(){
		 return "DisplayTopNavigation";
	 }


private:

	 static void event_handler(
			 Component * object,
			 const Event & event){
		 object->reinterpret<Display>()->local_event_handler(event);
	 }

	 void local_event_handler(const Event & event);

};

#endif // DISPLAY_HPP
