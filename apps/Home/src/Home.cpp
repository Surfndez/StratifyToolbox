#include "Home.hpp"
#include "Application.hpp"
#include "WidgetWindow.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application & application)
	: toolbox::ApplicationLayout<Application>("Home", application){

	const ux::DrawingArea control_icon_area(1000,250);
	const u8 control_border_size = 0;
	const enum Theme::style control_icon_style = Theme::style_brand_primary;

	add_component(
				(* new ux::Layout("ButtonLayout", event_loop()))
				.set_flow(ux::Layout::flow_vertical)
				.set_drawing_area(DrawingArea(250, 1000))
				.set_drawing_point(DrawingPoint(750,0))
				.add_component(
					Component::create<Button>("WifiButton")
					.set_border_size(control_border_size)
					.set_icon_name("wifi")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
					)
				.add_component(
					Component::create<Button>("LaunchButton")
					.set_border_size(control_border_size)
					.set_icon_name("rocket")
					.set_theme_style(control_icon_style)
					.set_drawing_area(control_icon_area)
				#if 0
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
			#endif
				)
			.add_component(
				Component::create<Button>("EjectButton")
				.set_border_size(control_border_size)
				.set_icon_name("eject")
				.set_theme_style(control_icon_style)
				.set_drawing_area(control_icon_area)
				)
			.add_component(
				Component::create<Button>("SettingsButton")
				.set_border_size(control_border_size)
				.set_icon_name("wrench")
				.set_theme_style(control_icon_style)
				.set_drawing_area(control_icon_area)
			#if 0
				.set_event_handler([&](Component * object, const Event & event){

					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.id() == ButtonEvent::id_released) && button_event.name() == "SettingsButton" ){
							printer().info("launch settings");
							Application::launch("/home/Settings", String() );
						}

					}


				})
		#endif
			)
			);



}



