#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;

class Configuration : public ApplicationLayout<Configuration> {
public:
	 Configuration(Application * application);

	 void set_io_pin(enum Io::io_pins pin){
		 m_io_pin = pin;
	 }

	 static const char * whatis_name(){
		 return "Configuration";
	 }

	 static const char * top_navigation_name(){
		 return "ConfigurationTopNavigation";
	 }

	 static const char * triangle_button_name(){
		 return "TriangleBtn";
	 }

	 static const char * square_button_name(){
		 return "SquareBtn";
	 }

	 static const char * wave_button_name(){
		 return "WaveBtn";
	 }

	 static const char * direction_value_name(){
		 return "DirectionValue";
	 }

private:
	 enum Io::io_pins m_io_pin;

	 static void event_handler(
			 Component * object,
			 const Event & event){
		 object->reinterpret<Configuration>()->local_event_handler(event);
	 }

	 void local_event_handler(const Event & event);

	 void update_display_values();
	 void toggle_direction();

};

#endif // CONFIGURATION_HPP
