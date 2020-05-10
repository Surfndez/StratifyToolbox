#include "PinConfiguration.hpp"
#include "Application.hpp"
#include "Control.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/Io.hpp>
#include <ToolboxAPI/components.hpp>

Configuration::Configuration(Application * application)
	: ApplicationLayout(whatis_name(), application){


	const drawing_size_t navigation_bar_height = 175;
	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("chevron-left")
					.set_title("Pin Settings"),
					event_loop()
					)
				.set_drawing_area(DrawingArea(1000,navigation_bar_height))
				);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);

	add_component(
				Layout::create(
					"PinConfigurationVerticalLayout",
					event_loop())
				.set_flow(ux::Layout::flow_vertical)
				.set_vertical_scroll_enabled()

				.add_component(
					Label::create("PinLabel")
					.set_label("Pin")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Label::create("PinValue")
					.set_label("?")
					.set_align_left()
					.set_left_margin(2)
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("ValueLabel")
					.set_label("Value")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Label::create("ValueValue")
					.set_label("---")
					.set_align_left()
					.set_left_margin(2)
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("DirectionLabel")
					.set_label("Direction")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Layout::create("DirectionLayout", event_loop())
					.set_flow(Layout::flow_horizontal)

					.add_component(
						Label::create(direction_value_name())
						.set_label("Output")
						.set_align_left()
						.set_left_margin(5)
						.set_border_size(0)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_area(DrawingArea(600,1000))
						)

					.add_component(
						Button::create(direction_button_name())
						.set_label("Toggle")
						.set_border_size(1)
						.set_right_margin(10)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_area(DrawingArea(400,1000))
						)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("FunctionLabel")
					.set_label("Function")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Label::create("FunctionValue")
					.set_label("GPIO")
					.set_align_left()
					.set_left_margin(2)
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(value_area)
					)


				.set_drawing_point(
					DrawingPoint(
						0, navigation_bar_height
						)
					)
				.set_drawing_area(
					DrawingArea(1000,1000-navigation_bar_height)
					)

				);

	set_event_handler(event_handler);


}

void Configuration::local_event_handler(const Event & event){


	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_enter ){
			update_display_values();
		}
		return;
	}

	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button ){
		if( button->name() ==
				find<TopNavigation>(top_navigation_name())->left_button_name()
				){
			event_loop()->layout()->transition("Control");
		}

		if( button->name() == Configuration::direction_button_name()
				){
			toggle_direction();
		}

	}
}


void Configuration::update_display_values(){
	IoInfo info(m_io_pin);
	Io io(info.io_pin());

	if( io.is_output() ){
		find<Button>("DirectionButton")
				->set_theme_style(
					Theme::style_brand_secondary
					).redraw();
	} else {
		find<Button>("DirectionButton")
				->set_theme_style(
					Theme::style_outline_brand_secondary
					).redraw();
	}

	this->find<Label>(
				"PinValue"
				)->set_label(
				info.name()
				).redraw();

	this->find<Label>(
				"ValueValue"
				)->set_label(
				io.value() ? "High" : "Low"
										 ).redraw();

	this->find<Label>(
				"DirectionValue"
				)->set_label(
				io.is_output() ? "Output" : "Input"
												 ).redraw();

	this->find<Label>(
				"FunctionValue"
				)->set_label(
				io.get_pin_description().get_description()
				).redraw();

	enum Theme::states next_state;
	if( io.get_pin_description().get_description() != "gpio" ){
		next_state = Theme::state_disabled;
	} else {
		next_state = Theme::state_default;
	}
	this->find<Button>(direction_button_name())
			->set_theme_state(next_state)
			.redraw();


}

void Configuration::toggle_direction(){
	Label * pin_value_label =
			event_loop()->layout()->find<Label>("PinValue");
	if( pin_value_label == nullptr ){
		//assert here
		printf("Pin value not found\n");
		return;
	}

	IoInfo information(pin_value_label->label());
	if( information.is_valid() ){
		Io io(information.io_pin());

		Label * direction_label =
				event_loop()->layout()->find<Label>( Configuration::direction_value_name() );
		Button * toggle_button =
				event_loop()->layout()->find<Button>( Configuration::direction_button_name() );

		Button * control_button =
				event_loop()->layout()->find<Button>(
					Control::pin_button_name(information)
					);

		PinMarkerBar * pin_marker_bar =
				event_loop()->layout()->find<PinMarkerBar>(
					Control::pin_marker_bar_name()
					);


		if( io.is_output() ){
			io.set_input();
			direction_label->set_label("Input");
			toggle_button->set_theme_style(Theme::style_outline_brand_secondary);
			control_button->set_theme_style(Theme::style_outline_brand_secondary);
			pin_marker_bar->set_pin_marker_style(
						information.io_pin(),
						Theme::style_outline_brand_secondary
						);
		} else {
			io.set_output();
			direction_label->set_label("Output");
			toggle_button->set_theme_style(Theme::style_brand_secondary);
			control_button->set_theme_style(Theme::style_brand_secondary);
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




