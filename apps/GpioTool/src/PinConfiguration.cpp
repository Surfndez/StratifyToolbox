#include "PinConfiguration.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

PinConfiguration::PinConfiguration(Application & application)
	: toolbox::ApplicationLayout<Application>(application){

	add_component(
				"BackButton",
				(* new ux::Button())
				.set_border_size(1)
				.set_icon_name("chevron-left")
				.set_theme_style(Theme::style_danger)
				.set_drawing_point(DrawingPoint(0, 0))
				.set_drawing_area(DrawingArea(200, 200))
			);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);

	add_component(
				"VerticalLayout",
				(* new ux::Layout(event_loop()))
				.set_vertical_scroll_enabled()

				.add_component(
					"PinLabel",
					(* new ux::Label())
					.set_label("Pin")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"PinValue",
					(* new ux::Label())
					.set_label("?")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"ValueLabel",
					(* new ux::Label())
					.set_label("Value")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"ValueValue",
					(* new ux::Label())
					.set_label("---")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_primary)
					.set_drawing_area(value_area)
					)

				.add_component(
					"DirectionLabel",
					(* new ux::Label())
					.set_label("Direction")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					"DirectionLayout",
					(* new ux::Layout(event_loop()))
					.set_flow(Layout::flow_horizontal)

					.add_component(
						"DirectionValue",
						(* new ux::Label())
						.set_label("Output")
						.set_align_left()
						.set_border_size(0)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(600,1000))
						)

					.add_component(
						"DirectionButton",
						(* new ux::Button())
						.set_label("Toggle")
						.set_border_size(1)
						.set_theme_style(Theme::style_outline_brand_primary)
						.set_drawing_area(DrawingArea(400,1000))
					)
				.set_drawing_area(value_area)
				)

			.add_component(
				"FunctionLabel",
				(* new ux::Label())
				.set_label("Function")
				.set_align_left()
				.set_border_size(0)
				.set_theme_style(Theme::style_light)
				.set_drawing_area(label_area)
				)

			.add_component(
				"FunctionValue",
				(* new ux::Label())
				.set_label("GPIO")
				.set_align_left()
				.set_border_size(0)
				.set_theme_style(Theme::style_outline_brand_primary)
				.set_drawing_area(value_area)
				)


			.set_drawing_point(
				DrawingPoint(
					25, 225
					)
				)
			.set_drawing_area(
				DrawingArea(950,775)
				)

	);


}



