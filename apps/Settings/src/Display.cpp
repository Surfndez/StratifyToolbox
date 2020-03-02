#include "Display.hpp"
#include "Application.hpp"

#include <ToolboxAPI/components.hpp>
#include <sapi/ux.hpp>

Display::Display(Application & application)
	: toolbox::ApplicationLayout<Application>("Display", application){


	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_title("Display"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	const drawing_size_t icon_size = 500;
	add_component(
				Component::create<Button>("SunButton")
				.set_icon_name("sun")
				.set_border_size(0)
				.set_horizontal_padding(0)
				.set_vertical_padding(0)
				.set_theme_style(Theme::style_outline_warning)
				.set_drawing_point(DrawingPoint(500-icon_size/2,500-icon_size/2))
				.set_drawing_area(DrawingArea(icon_size,icon_size))
				);


	add_component(
				Component::create<Slider>("BrightnessSlider")
				.set_value(25)
				.set_maximum(100)
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(ux::DrawingPoint(100,750))
				.set_drawing_area(ux::DrawingArea(800,100))
			);

	set_event_handler(event_handler);
}

void Display::local_event_handler(const Event & event){
	if( (event.type() == ButtonEvent::event_type()) &&
			(event.id() == ButtonEvent::id_released) ){
		const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();
		if( button_event.name() ==
				find<TopNavigation>(top_navigation_name()
					)->left_button_name() ){
			event_loop()->layout()->transition("Settings");
		}
	}
}



