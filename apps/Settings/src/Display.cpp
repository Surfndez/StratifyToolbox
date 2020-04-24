#include "Display.hpp"
#include "Application.hpp"

#include <ToolboxAPI/components.hpp>
#include <sapi/ux.hpp>

Display::Display(Application* application)
	: toolbox::ApplicationLayout<Display>("Display", application){

	set_flow(flow_vertical);

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_title("Display"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	add_component(
				Label::create("SunLabel")
				.set_icon_name("sun")
				.set_border_size(0)
				.set_horizontal_margin(20)
				.set_vertical_margin(20)
				.set_theme_style(Theme::style_outline_warning)
				//.set_drawing_point(0,175)
				.set_drawing_area(1000,500)
				);

	add_component(
				Slider::create("BrightnessSlider")
				.set_horizontal_margin(10)
				.set_vertical_margin(40)
				.set_value(25)
				.set_maximum(100)
				.set_theme_style(Theme::style_brand_primary)
				//.set_drawing_point(0,175+500)
				.set_drawing_area(1000,1000-(175+500))
				);

	set_event_handler(event_handler);
}

void Display::local_event_handler(const Event & event){

	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);
	if( button && button->name() ==
			find<TopNavigation>(
				top_navigation_name()
				)->left_button_name() ){
		event_loop()->layout()->transition("Settings");
	}

}



