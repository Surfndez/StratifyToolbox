#include "Display.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>

Display::Display(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);

   add_background_component(
            &(* new Button())
            .set_icon_name("sun")
            .set_border_size(0)
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(250,150))
            .set_drawing_area(DrawingArea(500,500))
            );


   add_component(
            "BrightnessSlider",
            &(* new ux::Slider())
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

   add_component(
            "BackButton",
            &(* new Button())
            .set_icon_name("chevron-left")
            .set_border_size(0)
            .set_theme_style(Theme::style_outline_brand_primary)
            .set_drawing_point(DrawingPoint(0,0))
            .set_drawing_area(DrawingArea(150,200))
            .set_event_handler(
               [&](Component * object, const Event & event){

               if( event.type() == ButtonEvent::event_type() ){
                  const ButtonEvent & button_event = event.reinterpret<ButtonEvent>();

                  if( (button_event.name() == "BackButton") &&
                  (button_event.id() == ButtonEvent::id_released) ){
                     printf("back button pressed\n");
                     scene_collection()->set_current_scene("Settings");
                  }
               }
            })
         );

   //just right of the button
   add_background_component(
            &(* new Label())
            .set_label("Settings")
            .set_border_size(0)
            .set_align_left()
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(150,0))
            .set_drawing_area(DrawingArea(750,200))
            );

   //just right of the button
   add_background_component(
            &(* new Label())
            .set_label("Display")
            .set_border_size(0)
            .set_align_right()
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(600,0))
            .set_drawing_area(DrawingArea(375,200))
            );

}



