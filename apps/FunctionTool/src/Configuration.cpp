#include "Configuration.hpp"
#include "Application.hpp"
#include "Control.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/Io.hpp>
#include <ToolboxAPI/components.hpp>

Configuration::Configuration(Application & application)
	: ApplicationLayout<Application>(whatis_name(), application){


	const drawing_size_t navigation_bar_height = 175;
	add_component(
				(* new TopNavigation(
					 top_navigation_name(),
					 TopNavigationAttributes()
					 .set_left_icon_name("chevron-left")
					 .set_title("Settings"),
					 event_loop()
					 )
				 )
				.set_drawing_area(DrawingArea(1000,navigation_bar_height))
				);

	DrawingArea label_area(1000,150);
	DrawingArea value_area(1000,200);
	DrawingArea button_area(1000,300);

	add_component(
				(* new ux::Layout(
					 "ConfigurationVerticalLayout",
					 event_loop()))
				.set_flow(ux::Layout::flow_vertical)
				.set_vertical_scroll_enabled()

				.add_component(
					Component::create<ux::Label>("PinLabel")
					.set_label("Pin")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Component::create<ux::Label>("PinValue")
					.set_label("?")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Component::create<ux::Label>("Function")
					.set_label("Function")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					(* new ux::Layout("FunctionLayout", event_loop()))

					.add_component(
						Component::create<ux::Button>(triangle_button_name())
						.set_label("Tri")
						.set_border_size(1)
						.set_vertical_padding(30)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_point(DrawingPoint(25,0))
						.set_drawing_area(DrawingArea(300,1000))
						)

					.add_component(
						Component::create<ux::Button>(wave_button_name())
						.set_label("Sin")
						.set_border_size(1)
						.set_vertical_padding(30)
						.set_theme_style(Theme::style_outline_brand_secondary)
						.set_drawing_point(DrawingPoint(500 - 300/2, 0))
						.set_drawing_area(DrawingArea(300,1000))
						)

					.add_component(
						Component::create<ux::Button>(square_button_name())
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
					Component::create<ux::Label>("FrequencyLabel")
					.set_label("Frequency")
					.set_align_left()
					.set_border_size(0)
					.set_theme_style(Theme::style_light)
					.set_drawing_area(label_area)
					)

				.add_component(
					Component::create<ux::Slider>("FrequencySlider")
					.set_maximum(1000)
					.set_value(500)
					.set_theme_style(Theme::style_brand_secondary)
					.set_drawing_area(value_area)
					)

				.add_component(
					Component::create<ux::Label>("FrequencyValue")
					.set_label("500Hz")
					.set_align_center()
					.set_border_size(0)
					.set_theme_style(Theme::style_outline_brand_secondary)
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

	set_event_handler([&application, this](Component * object, const Event & event){

		if( event.type() == SystemEvent::event_type() ){
			if( event.id() == SystemEvent::id_enter ){
				update_display_values();
			}
		}

		if( event.type() == ButtonEvent::event_type() ){
			if( event.id() == ButtonEvent::id_released ){
				const ButtonEvent	& button_event= event.reinterpret<ButtonEvent>();
				if( button_event.name() ==
						find<TopNavigation>(top_navigation_name())->left_button_name()
						){
					event_loop()->layout()->transition("Control");
				} else if( button_event.name() == triangle_button_name() ){
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
				} else if( button_event.name() == square_button_name() ){
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
				} else if( button_event.name() == wave_button_name() ){
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

	});


}


void Configuration::update_display_values(){
	IoInfo info(m_io_pin);


	find<Label>("PinValue")
			->set_label(info.name()
									).redraw();

}





