#include "TopNavigation.hpp"

TopNavigation::TopNavigation(const var::String& name){

	add_sub_component(
				"BackButton",
				(* new Button())
				.set_icon_name("chevron-left")
				.set_border_size(0)
				.set_theme_style(Theme::style_brand_primary)
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(200,1000))
				.set_event_handler(
					[&](Component * object, const Event & event){

					if( event.type() == ButtonEvent::event_type() ){
						const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

						if( (button_event.name() == "BackButton") &&
						(button_event.id() == ButtonEvent::id_released) ){
							scene_collection()->set_current_scene("Settings");
						}
					}
				})
			);

	add_sub_component(
				"Label",
				(* new Label())
				.set_label(name)
				.set_border_size(0)
				.set_align_right()
				.set_padding(30)
				.set_theme_style(Theme::style_light)
				.set_drawing_point(DrawingPoint(200,0))
				.set_drawing_area(DrawingArea(800,1000))
				);

	set_drawing_point(DrawingPoint(0,0));
	set_drawing_area(DrawingArea(1000, 200));

}
