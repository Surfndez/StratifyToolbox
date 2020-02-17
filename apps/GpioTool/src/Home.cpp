#include "Home.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	event_loop()->set_update_period(chrono::Milliseconds(10));

	add_component(
				"ControlButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_label("IO")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(150, 150))
				.set_drawing_area(DrawingArea(300, 300))
				.set_event_handler([&](Component * object, const Event & event){
					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "ControlButton" ){
							//scene_collection()->set_current_scene("Control");
						}
					}
				})
				);

	add_component(
				"ConfigurationButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("sliders-h")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(550, 150))
				.set_drawing_area(DrawingArea(300, 300))
				.set_event_handler([&](Component * object, const Event & event){
					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "ConfigurationButton" ){
							//scene_collection()->set_current_scene("Configuration");
						}
					}
				})
				);

	add_component(
				"AboutButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("info")
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(150, 550))
				.set_drawing_area(DrawingArea(300, 300))
				.set_event_handler([&](Component * object, const Event & event){
					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "AboutButton" ){
							//scene_collection()->set_current_scene("About");
						}
					}
				})
				);

	add_component(
				"ExitButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_label("EXIT")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(550, 550))
				.set_drawing_area(DrawingArea(300, 300))
				.set_event_handler([&](Component * object, const Event & event){
					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "ExitButton" ){
							Application::go_home();
						}
					}
				})
				);

}



