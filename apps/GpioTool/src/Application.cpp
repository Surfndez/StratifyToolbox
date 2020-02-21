
#include <sapi/ux.hpp>
#include <sapi/var.hpp>
#include <ToolboxAPI/components.hpp>

#include "Application.hpp"
#include "Home.hpp"
#include "Configuration.hpp"
#include "PinConfiguration.hpp"
#include "About.hpp"
#include "Control.hpp"

Application::Application(const sys::Cli & cli)
	: ToolboxApplication(cli)
{

}

ux::Layout & Application::create_layout(){

	return (Layout&)(*(new Layout(event_loop())))
			.add_component(
				"Control",
				(* new Control(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				)
			.add_component(
				"PinConfiguration",
				(* new PinConfiguration(*this))
				.set_drawing_point(
					DrawingPoint(0,0)
					)
				.set_drawing_area(
					DrawingArea(1000,1000)
					)
				.set_enabled(false)
				)
			.set_event_handler([this](ux::Component * object,
												 const ux::Event & event){
		Application::handle_application_event(*this, object, event);
	}
	)
	.set_drawing_area(
	DrawingArea(1000,1000)
	);

}


void Application::handle_application_event(
		Application & application,
		ux::Component * object,
		const ux::Event & event
		){

	Layout * layout = object->reinterpret<Layout>();
	if( event.type() == ButtonEvent::event_type() ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

		if( button_event.id() == ButtonEvent::id_released ){
			if( (button_event.name() == "ConfigurationButton") ||
					(button_event.name() == "BackConfiguration") ){
				layout->transition("Configuration");
			} else if( button_event.name() == "ControlButton" ){
				layout->transition("Control");
			} else if( button_event.name() == "AboutButton" ){
				//layout->transition("Home");
			} else if( button_event.name() == "ExitButton" ){
				//launch the home applicatione
				go_home();
			} else if( button_event.name() == "BackHome" ){
				layout->transition("Home");
			} else if( button_event.name().find("Configure") == 0 ){
				//update the data model with the currently selected pin

				model().insert(
							"pinConfiguration",
							JsonString(button_event.name())
							);

				layout->transition("PinConfiguration");

			} else if( button_event.name() == "DirectionButton" ){
				application.toggle_direction();
			}
		}

	}

}

void Application::toggle_direction(){
	Label * pin_value_label =
			event_loop()->layout()->find<Label>("PinValue");
	if( pin_value_label == nullptr ){
		//assert here
		printf("Pin value not found\n");
		return;
	}

	IoInformation information(pin_value_label->label());
	if( information.is_valid() ){
		printer().info("Toggle IO for " + information.name());
		Io io(information.io_pin());

		Label * direction_label =
				event_loop()->layout()->find<Label>("DirectionValue");
		Button * toggle_button =
				event_loop()->layout()->find<Button>("DirectionButton");
		Button * control_button =
				event_loop()->layout()->find<Button>(
					Control::pin_button_name(information)
					);
		Button * configuration_button =
				event_loop()->layout()->find<Button>(
					Configuration::pin_button_name(information)
					);
		PinMarkerBar * pin_marker_bar =
				event_loop()->layout()->find<PinMarkerBar>("PinMarkerBar");


		if( io.is_output() ){
			io.set_input();
			direction_label->set_label("Input");
			toggle_button->set_theme_style(Theme::style_outline_brand_secondary);
			control_button->set_theme_style(Theme::style_outline_brand_secondary);
			configuration_button->set_theme_style(Theme::style_outline_brand_secondary);
			pin_marker_bar->set_pin_marker_style(
						information.io_pin(),
						Theme::style_outline_brand_secondary
						);
		} else {
			io.set_output();
			direction_label->set_label("Output");
			toggle_button->set_theme_style(Theme::style_brand_secondary);
			control_button->set_theme_style(Theme::style_brand_secondary);
			configuration_button->set_theme_style(Theme::style_brand_secondary);
			pin_marker_bar->set_pin_marker_style(
						information.io_pin(),
						Theme::style_brand_secondary
						);
		}
		direction_label->redraw();
		toggle_button->redraw();
		control_button->redraw();
	}

}


