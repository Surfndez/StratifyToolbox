#include "Keyboard.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>
#include <ToolboxAPI/toolbox.hpp>
#include <ToolboxAPI/components.hpp>


Keyboard::Keyboard(Application & app)
	: ApplicationLayout<Application>("Keyboard", app){

	m_button_set = 0;
	m_button_options.at(0) = "qwertyuiopasdfghjklzxcvbnm";
	m_button_options.at(1) = "QWERTYUIOPASDFGHJKLZXCVBNM";
	m_button_options.at(2) = "1234567890!@#$%&*()^-=+`~_";
	m_button_options.at(3) = "1234567890{}[]|;:'\"<>,./?\\";
	m_text = "|";

	constexpr drawing_size_t navigation_bar_size = 175;

	add_component(
				Component::create<TopNavigation>(
					top_navigation_name(),
					TopNavigationAttributes()
					.set_left_icon_name("check")
					.set_right_icon_name("times")
					.set_title("..."),
					event_loop())
				.set_drawing_area(DrawingArea(1000,navigation_bar_size))
				);

	constexpr u32 row_count = 3;
	constexpr u32 column_count = 10;

	constexpr drawing_size_t button_row_offset = 10;
	constexpr drawing_size_t button_column_offset = 5;
	constexpr drawing_size_t intra_button_spacing = 5;
	constexpr drawing_size_t button_column_spacing = (1000 - 2*button_column_offset)/column_count;
	constexpr drawing_size_t button_row_spacing = ((1000 - navigation_bar_size - 2*button_row_offset))/row_count;
	constexpr drawing_size_t button_width = button_column_spacing - intra_button_spacing;
	constexpr drawing_size_t button_height = button_row_spacing - intra_button_spacing;

	const DrawingArea button_area(button_width, button_height);

	u32 row = 0;
	u32 column = 0;
	drawing_size_t column_bump_spacing = 0;

	for(u32 i=0; i < key_count(); i++){


		if( column == 0 ){
			if( row == 1 ){
				column_bump_spacing += button_column_spacing/2;
			} else if( row == 2 ){
				column_bump_spacing += button_column_spacing;
			}
		}

		add_component(
					Component::create<Button>(
						"key:" + String::number(i)
						)
					.set_label(String(button_set().at(i)))
					.set_vertical_padding(30)
					.set_drawing_point(
						DrawingPoint(
							column * button_column_spacing + button_column_offset + intra_button_spacing/2 + column_bump_spacing,
							row * button_row_spacing + button_row_offset + intra_button_spacing/2 + navigation_bar_size
							)
						)
					.set_theme_style(Theme::style_outline_brand_secondary)
					.set_drawing_area(button_area)
					);

		column++;
		if( row == 0 ){
			if( column == column_count ){
				row++;
				column = 0;
			}
		} else {
			if( column == column_count - 1 ){
				row++;
				column = 0;
			}
		}
	}

	add_component(
				Component::create<Button>(
					"space"
					)
				.set_label("")
				.set_drawing_point(
					DrawingPoint(
						column * button_column_spacing + button_column_offset + intra_button_spacing/2 + column_bump_spacing,
						row * button_row_spacing + button_row_offset + intra_button_spacing/2 + navigation_bar_size
						)
					)
				.set_drawing_area(
					DrawingArea(
						button_width*3/2,
						button_height
						)
					)
				.set_theme_style(Theme::style_outline_brand_secondary)
				);

	column = 0;
	column_bump_spacing = 0;

	add_component(
				Component::create<Button>(
					"shift"
					)
				.set_icon_name("arrow-up")
				.set_vertical_padding(30)
				.set_drawing_point(
					DrawingPoint(
						column * button_column_spacing + button_column_offset + intra_button_spacing/2 + column_bump_spacing,
						row * button_row_spacing + button_row_offset + intra_button_spacing/2 + navigation_bar_size
						)
					)
				.set_theme_style(Theme::style_outline_brand_secondary)
				.set_drawing_area(
					DrawingArea(
						button_width*3/2,
						button_height
						)
					)
				);
	set_event_handler(Keyboard::event_handler);
}

void Keyboard::local_event_handler(
		const ux::Event & event
		){

	Button * button = ButtonEvent::component(event, ButtonEvent::id_released);

	if( button ){
		if( button->name() == "shift" ){
			m_button_set++;
			if( m_button_set == m_button_options.count() ){
				m_button_set = 0;
			}
			update_symbols();
		} else if( button->name() == "space" ){
			m_text << " ";
			update_text();
		} else if( button->name() == find<TopNavigation>(top_navigation_name())->left_button_name() ){
			application().go_home();
		} else if( button->name() == find<TopNavigation>(top_navigation_name())->right_button_name() ){
			if( m_text.length() > 0 ){
				m_text.erase(
							String::Position(m_text.length()-1)
							);
				update_text();
			}
		} else {
			if( button->name().find("key:") == 0 ){
				var::Vector<String> values = button->name().split(":");
				u32 key_index = values.at(1).to_integer();
				char c = button_set().at(key_index);
				m_text << c;
				update_text();
			}
		}
	}
}

void Keyboard::update_text(){
	m_display_text = m_text + "|";

	find<Label>(
				find<TopNavigation>(top_navigation_name())->title_name()
				)->set_label(m_display_text).redraw();
}


void Keyboard::update_symbols(){

	if( button_set().length() != key_count() ){
		printer().error(
					"button set " +
					button_set() +
					" has bad length  "
					+ String::number(button_set().length())
					);
	}

	for(u32 i=0; i < key_count(); i++){

		String key = "key:" + String::number(i);
		Button * button = find<Button>(key);
		if( button == nullptr ){
			printer().error("failed to find key " + key);
		} else {
			button->set_label(
						String(button_set().at(i))
						).redraw();
		}

	}
}



