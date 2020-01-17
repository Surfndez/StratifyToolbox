#include "Display.hpp"
#include "Application.hpp"
#include "TopNavigation.hpp"

#include <sapi/ux.hpp>

Display::Display(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);

	 add_component(
				 "TopNavigation",
				 (* new TopNavigation("Display"))
				 );

   add_background_component(
						(* new Button())
            .set_icon_name("sun")
            .set_border_size(0)
            .set_theme_style(Theme::style_outline_warning)
            .set_drawing_point(DrawingPoint(250,200))
            .set_drawing_area(DrawingArea(500,500))
            );


   add_component(
            "BrightnessSlider",
						(* new ux::Slider())
            .set_value(25)
            .set_maximum(100)
            .set_theme_style(Theme::style_brand_primary)
            .set_drawing_point(ux::DrawingPoint(100,750))
            .set_drawing_area(ux::DrawingArea(800,100))
            .set_antialias(false)
            .set_event_handler(
               [&](ux::Component * object, const ux::Event & event){

            })
         );

}



