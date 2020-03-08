#ifndef HOME_HPP
#define HOME_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/ToolboxApplication.hpp>

class Application;

class Home : public toolbox::ApplicationLayout<Home> {
public:
	 Home(Application * application);
   //~Home();



private:

	 static void event_handler(
			 Component * object,
			 const Event & event){
		 object->reinterpret<Home>()->local_event_handler(event);
	 }

	 void local_event_handler(const Event & event);

};

#endif // HOME_HPP
