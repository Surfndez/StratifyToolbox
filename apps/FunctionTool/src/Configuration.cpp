#include "Configuration.hpp"
#include "Application.hpp"
#include "Control.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/Io.hpp>
#include <ToolboxAPI/components.hpp>

Configuration::Configuration(Application* application)
	: ApplicationLayout(whatis_name(), application){


	const drawing_size_t navigation_bar_height = 175;
	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("chevron-left")
					.set_title("Settings"),
					event_loop()
					)
				.set_drawing_area(DrawingArea(1000,navigation_bar_height))
				);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);
	DrawingArea button_area(1000,300);

	add_component(
				Layout::create(
					"ConfigurationVerticalLayout",
					event_loop()
					)
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
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("Function")
					.set_label("Function")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Layout::create("FunctionLayout", event_loop())
					.add_component(
						Button::create(triangle_button_name())
						.set_label("Tri")
						.set_border_size(1)
						.set_vertical_padding(30)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_point(DrawingPoint(25,0))
						.set_drawing_area(DrawingArea(300,1000))
						)

					.add_component(
						Button::create(wave_button_name())
						.set_label("Sin")
						.set_border_size(1)
						.set_vertical_padding(30)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_point(DrawingPoint(500 - 300/2, 0))
						.set_drawing_area(DrawingArea(300,1000))
						)

					.add_component(
						Button::create(square_button_name())
						.set_label("Sq")
						.set_border_size(1)
						.set_vertical_padding(30)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_point(DrawingPoint(975-300, 0))
						.set_drawing_area(DrawingArea(300,1000))
						)
					.set_drawing_area(button_area)

					)

				.add_component(
					Label::create("FrequencyLabel")
					.set_label("Frequency: 500hz")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Slider::create("FrequencySlider")
					.set_maximum(1000)
					.set_value(500)
					.set_theme_style(Theme::style_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("AmplitudeLabel")
					.set_label("Amplitude: 0.0")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Slider::create("AmplitudeSlider")
					.set_maximum(1000)
					.set_value(500)
					.set_theme_style(Theme::style_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Label::create("OffsetLabel")
					.set_label("Offset: 0.0")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Slider::create("OffsetSlider")
					.set_maximum(1000)
					.set_value(500)
					.set_theme_style(Theme::style_brand_secondary)
					.set_drawing_area(value_area)
					)


				.set_drawing_point(
					DrawingPoint(
						25, navigation_bar_height
						)
					)
				.set_drawing_area(
					DrawingArea(950,1000-navigation_bar_height)
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

	Slider * slider = SliderEvent::component(event);
	if( slider ){
		if( slider->name() == "FrequencySlider" ){
			find<Label>("FrequencyLabel")
					->set_label(
						"Frequency: " + String::number(
							slider->value()
							) + "hz").redraw();
		} else if( slider->name() == "AmplitudeSlider" ){
			find<Label>("AmplitudeLabel")
					->set_label(
						"Amplitude: " + String::number(
							slider->value() * 2.0f / slider->maximum() -1.0f
							) + " ").redraw();
		} else if( slider->name() == "OffsetSlider" ){
			find<Label>("OffsetLabel")
					->set_label(
						"Offset: " + String::number(
							slider->value() * 2.0f / slider->maximum() -1.0f
							) + " ").redraw();
		}
		return;
	}

	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button ){
		if( button->name() ==
				find<TopNavigation>(top_navigation_name())->left_button_name()
				){
			event_loop()->layout()->transition("Control");
		} else if( button->name() == triangle_button_name() ){
			find<Button>(triangle_button_name())
					->set_theme_style(
						Theme::style_brand_secondary
						).redraw();

			find<Button>(square_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();

			find<Button>(wave_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();

		} else if( button->name() == square_button_name() ){
			find<Button>(square_button_name())
					->set_theme_style(
						Theme::style_brand_secondary
						).redraw();

			find<Button>(triangle_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();

			find<Button>(wave_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();
		} else if( button->name() == wave_button_name() ){
			find<Button>(wave_button_name())
					->set_theme_style(
						Theme::style_brand_secondary
						).redraw();

			find<Button>(triangle_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();

			find<Button>(square_button_name())
					->set_theme_style(
						Theme::style_outline_brand_secondary
						).redraw();
		}
	}
}


void Configuration::update_display_values(){
	IoInfo info(m_io_pin);


	find<Label>("PinValue")
			->set_label(info.name()
									).redraw();

}





