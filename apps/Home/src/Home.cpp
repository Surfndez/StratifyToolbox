#include "Home.hpp"
#include "Application.hpp"
#include "WidgetWindow.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);

	const ux::DrawingArea control_icon_area(200,250);
	const ux::DrawingPoint control_icon_start(800,0);
	const drawing_size_t control_icon_spacing = 250;
	const u8 control_border_size = 0;
	const enum Theme::style control_icon_style = Theme::style_brand_primary;

	add_background_component(
				(* new WidgetWindow("Widget1"))
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(800,333))
				);

	add_background_component(
				(* new WidgetWindow("Widget2"))
				.set_drawing_point(DrawingPoint(0,333))
				.set_drawing_area(DrawingArea(800,333))
				);

	add_background_component(
				(* new WidgetWindow("Widget3"))
				.set_drawing_point(DrawingPoint(0,666))
				.set_drawing_area(DrawingArea(800,333))
				);

#if 0
	add_component(
				"Outline",
				(* new Label())
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(800,333))
				.set_theme_style(Theme::style_brand_secondary)
				);

	add_component(
				"Outline",
				(* new Label())
				.set_drawing_point(DrawingPoint(100,25))
				.set_drawing_area(DrawingArea(950,850))
				.set_theme_style(Theme::style_light)
				);
#endif


	add_component(
				"WifiButton",
				(* new ux::Button())
				.set_border_size(control_border_size)
				.set_icon_name("wifi")
				.set_theme_style(control_icon_style)
				.set_drawing_point(control_icon_start)
				.set_drawing_area(control_icon_area)
				);

	add_component(
				"LaunchButton",
				(* new ux::Button())
				.set_border_size(control_border_size)
				.set_icon_name("rocket")
				.set_theme_style(control_icon_style)
				.set_drawing_point(control_icon_start + ux::DrawingPoint::Y(control_icon_spacing))
				.set_drawing_area(control_icon_area)
				.set_event_handler([&](Component * object, const Event & event){

					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "SettingsButton" ){
							printer().info("launch settings");
							Application::launch("/home/Settings", String() );
						}

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "LaunchButton" ){
							printer().info("launch Launcher");
							Application::launch("/home/Launcher", String() );
						}

					}

				})
			);

	add_component(
				"EjectButton",
				(* new ux::Button())
				.set_border_size(control_border_size)
				.set_icon_name("eject")
				.set_theme_style(control_icon_style)
				.set_drawing_point(control_icon_start + ux::DrawingPoint::Y(2*control_icon_spacing))
				.set_drawing_area(control_icon_area)
				);

	add_component(
				"SettingsButton",
				(* new ux::Button())
				.set_border_size(control_border_size)
				.set_icon_name("wrench")
				.set_theme_style(control_icon_style)
				.set_drawing_point(control_icon_start + ux::DrawingPoint::Y(3*control_icon_spacing))
				.set_drawing_area(control_icon_area)
				.set_event_handler([&](Component * object, const Event & event){

					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "SettingsButton" ){
							printer().info("launch settings");
							Application::launch("/home/Settings", String() );
						}

					}


				})
			);





}



