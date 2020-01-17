#include "WidgetWindow.hpp"

WidgetWindow::WidgetWindow(
		const var::String & name){

	add_sub_component(
				"Outline",
				(* new Label())
				.set_drawing_point(DrawingPoint(0,0))
				.set_drawing_area(DrawingArea(1000,1000))
				.set_theme_style(Theme::style_brand_secondary)
				);

	add_sub_component(
				"Outline",
				(* new Label())
				.set_drawing_point(DrawingPoint(25,150))
				.set_drawing_area(DrawingArea(950,775))
				.set_theme_style(Theme::style_light)
				);

}
