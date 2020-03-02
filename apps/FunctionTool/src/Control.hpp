#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>

class Application;

class Wave {
public:
	Wave(){}

	Wave(const var::String & name,
			 LineGraphDataSet && data_set
			 ) :
		m_name(name){
		m_data_set = std::move(data_set);
	}

	const var::String & name() const {
		return m_name;
	}

	void set_name(const var::String & value){
		m_name = value;
	}

	LineGraphDataSet & data_set(){
		return m_data_set;
	}
	const LineGraphDataSet & data_set() const {
		return m_data_set;
	}

private:
	var::String m_name;
	LineGraphDataSet m_data_set;
};

class Control : public ApplicationLayout<Application> {
public:
	Control(Application & app);

	enum output {
		output_zero,
		output_one,
		last_output = output_one
	};

	static const char * top_navigation_name(){
		return "ControlTopNavigation";
	}

	static const char * line_graph0_name(){
		return "LineGraphTop";
	}

	static const char * line_graph1_name(){
		return "LineGraphTop";
	}

	static const char * pin_marker_bar_name(){
		return "PinMarkerBar";
	}

	Wave & wave(enum output output_value){ return m_wave.at(output_value); }
	const Wave & wave(enum output output_value) const {
		return m_wave.at(output_value);
	}

private:

	var::Array<Wave, last_output+1> m_wave;
	var::Array<s8, Io::last_io_pin> m_pin_states;
	static void event_handler(
			ux::Layout * object,
			const ux::Event & event
			){
		Control * control_object = object->reinterpret<Control>();
		control_object->local_event_handler(event);
	}
	void local_event_handler(const ux::Event & event);
	bool handle_io_button_press(const ux::ButtonEvent & button_event);
	void update_pin_markers();

	const LineGraphDataSet * create_triangle_wave_data(enum output output_value);
	const LineGraphDataSet * create_square_wave_data(enum output output_value);
	const LineGraphDataSet * create_sine_wave_data(enum output output_value);
	const LineGraphDataSet * create_flat_wave_data(enum output output_value);
};

#endif // CONTROL_HPP
