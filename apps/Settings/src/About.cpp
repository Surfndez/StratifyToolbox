#include "About.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>
#include <sapi/sys.hpp>

About::About(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);

   SysInfo info = Sys::get_info();

   add_background_component(
            &(* new Label())
            .set_label("Serial Number")
            .set_border_size(0)
            .set_align_left()
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(25,200))
            .set_drawing_area(DrawingArea(750,100))
            );

   add_background_component(
            &(* new Label())
            .set_font_name("robotoc")
            .set_label(info.serial_number().to_string())
            .set_border_size(0)
            .set_align_left()
            .set_antialias(false)
            .set_theme_style(Theme::style_outline_brand_primary)
            .set_drawing_point(DrawingPoint(50,300))
            .set_drawing_area(DrawingArea(950,75))
            );

   add_background_component(
            &(* new Label())
            .set_label("Version")
            .set_border_size(0)
            .set_align_left()
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(25,600))
            .set_drawing_area(DrawingArea(750,400))
            );

   add_component(
            "BackButton",
            &(* new Button())
            .set_icon_name("chevron-left")
            .set_border_size(0)
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(0,0))
            .set_drawing_area(DrawingArea(200,200))
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
            &(* new Button())
            .set_icon_name("Settings")
            .set_border_size(0)
            .set_theme_style(Theme::style_outline_dark)
            .set_drawing_point(DrawingPoint(250,0))
            .set_drawing_area(DrawingArea(750,200))
            );




}



