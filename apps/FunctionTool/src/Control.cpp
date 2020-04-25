#include <cmath>
#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>

#include "Control.hpp"
#include "Application.hpp"
#include "Configuration.hpp"


Control::Control(Application* app)
	: ApplicationLayout("Control", app){

	add_component(
				TopNavigation::create(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("times")
					.set_right_icon_name("info")
					.set_title("Function Tool"),
					event_loop())
				.set_drawing_area(DrawingArea(1000,175))
				);

	const DrawingArea button_area(500, 300);
	const DrawingArea label_area(500, 175);
	constexpr drawing_int_t button_y = 175;
	const drawing_int_t left_x = 0;
	const drawing_int_t right_x = 500;
	const drawing_int_t label_y = button_y + button_area.height();
	const drawing_int_t graph_y = label_y + label_area.height();
	constexpr u8 button_vertical_padding = 25;
	constexpr enum Theme::style output0_theme_style = Theme::style_brand_secondary;
	constexpr enum Theme::style output1_theme_style = Theme::style_info;

	add_component(
				Button::create(
					"output0"
					)
				.set_label("Output 0")
				.set_theme_style(output0_theme_style)
				.set_vertical_padding(button_vertical_padding)
				.set_horizontal_margin(20)
				.set_vertical_margin(20)
				.set_drawing_point(
					DrawingPoint(left_x, button_y)
					)
				.set_drawing_area(button_area)
				);

	add_component(
				Label::create(
					"status0"
					)
				.set_label("off")
				.set_theme_style(Theme::style_outline_dark)
				.set_vertical_padding(button_vertical_padding)
				.set_border_size(0)
				.set_drawing_point(
					DrawingPoint(left_x, label_y)
					)
				.set_drawing_area(label_area)
				);

	add_component(
				LineGraph::create(
					line_graph1_name()
					)
				.set_drawing_point(
					DrawingPoint(left_x,graph_y)
					)
				.set_horizontal_margin(10)
				.set_drawing_area(button_area)
				.set_data_set( create_sine_wave_data(output_zero) )
				.fit_axes_to_data()
				);

	add_component(
				Button::create(
					"output1"
					)
				.set_theme_style(output1_theme_style)
				.set_label("Output 1")
				.set_horizontal_margin(20)
				.set_vertical_margin(20)
				.set_vertical_padding(button_vertical_padding)
				.set_drawing_point(
					DrawingPoint(right_x, button_y)
					)
				.set_drawing_area(button_area)
				);

	add_component(
				Label::create(
					"status1"
					)
				.set_label("off")
				.set_theme_style(Theme::style_outline_dark)
				.set_vertical_padding(button_vertical_padding)
				.set_border_size(0)
				.set_drawing_point(
					DrawingPoint(right_x, label_y)
					)
				.set_drawing_area(label_area)
				);

	add_component(
				LineGraph::create(
					line_graph1_name()
					)
				.set_drawing_point(
					DrawingPoint(right_x,graph_y)
					)
				.set_horizontal_margin(10)
				.set_drawing_area(button_area)
				.set_data_set( create_flat_wave_data(output_one) )
				.fit_axes_to_data()
				);

	add_component(
				PinMarkerBar::create(
					String(pin_marker_bar_name()),
					event_loop(),
					IoInfo::type_analog
					)
				//disable ADC pins
				.set_pin_marker_enabled(Io::io_pin16, false)
				.set_pin_marker_enabled(Io::io_pin17, false)
				//match style of buttons
				.set_pin_marker_style(Io::io_pin18, output0_theme_style)
				.set_pin_marker_style(Io::io_pin19, output1_theme_style)
				//bottom 10% of screen (full width)
				.set_drawing_point(DrawingPoint(0,900))
				.set_drawing_area(DrawingArea(1000,100))
				);



	set_event_handler(Control::event_handler);

}

void Control::local_event_handler(
		const ux::Event & event
		){

	if( event.type() == SystemEvent::event_type() ){
		if( event.id() == SystemEvent::id_update ){
			update_pin_markers();
		} else if( event.id() == SystemEvent::id_enter ){
			//update status labels and line graphs

		}
		return;
	}

	Button * button = ButtonEvent::component(event);
	if( button ){

		if( event.id() == ButtonEvent::id_held ){
			if( button->name() == "output0" ){
				event_loop()->layout()->find<Configuration>("Configuration")
						->set_io_pin(
							Io::io_pin18
							);
				event_loop()->layout()->transition("Configuration");
			} else if( button->name() == "output1" ){
				event_loop()->layout()->find<Configuration>("Configuration")
						->set_io_pin(
							Io::io_pin19
							);
				event_loop()->layout()->transition("Configuration");
			}

		} else if( event.id() == ButtonEvent::id_released ){

			if( button->name() ==
					find<TopNavigation>(top_navigation_name())->right_button_name()
					){
				event_loop()->layout()->transition("About");
			} else if( button->name() ==
								 find<TopNavigation>(top_navigation_name())->left_button_name()
								 ){
				application()->go_home();
			}

		}
	}
}

bool Control::handle_io_button_press(const ux::Button * button){
	IoInfo information(button->name());
	if( information.is_valid() ){
		Io io(information);
		if( io.is_output() ){
			io << !io.value();
		}
		return true;
	}
	return false;
}


void Control::update_pin_markers(){


}

const LineGraphDataSet * Control::create_triangle_wave_data(
		enum output output_value
		){
	LineGraphData data(11);
	for(u32 i = 0; i < data.count(); i++){
		float x = i * 1.0f;
		float y = (i % 2) ? 1.0f : -1.0f;
		data.at(i) = LineGraphPoint(x,y);
	}
	return &wave(output_value).data_set();
}

const LineGraphDataSet * Control::create_square_wave_data(
		enum output output_value
		){
	LineGraphData data(22);
	for(u32 i = 0; i < data.count(); i++){
		float x = i/2 * 1.0f;
		float y = (((i+1) >> 1) & 1) ? 0.1f : 0.9f;
		data.at(i) = LineGraphPoint(x,y);
	}

	wave(output_value).data_set() = LineGraphDataSet();
	wave(output_value).data_set().push_back(data);

	return &wave(output_value).data_set();
}

const LineGraphDataSet * Control::create_sine_wave_data(
		enum output output_value
		){

	LineGraphData data(64);
	for(u32 i = 0; i < data.count(); i++){
		float x = i * 0.08f;
		float y = 1.0f * sinf(2.0f * MCU_PI_FLOAT * x);
		data.at(i) = LineGraphPoint(x,y);
	}

	wave(output_value).data_set() = LineGraphDataSet();
	wave(output_value).data_set().push_back(data);

	return &wave(output_value).data_set();
}


const LineGraphDataSet * Control::create_flat_wave_data(
		enum output output_value
		){
	LineGraphData data(2);
	data.at(0) = LineGraphPoint(0.0f, 0.0f);
	data.at(1) = LineGraphPoint(1.0f, 0.0f);
	wave(output_value).data_set() = LineGraphDataSet();
	wave(output_value).data_set().push_back(data);
	return &wave(output_value).data_set();
}




