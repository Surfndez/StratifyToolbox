#include "About.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>
#include <sapi/sys.hpp>

About::About(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);

   SysInfo info = Sys::get_info();

   add_component(
            "BackButton",
            &(* new Button())
            .set_icon_name("chevron-left")
            .set_border_size(0)
            .set_theme_style(Theme::style_brand_primary)
            .set_drawing_point(DrawingPoint(0,0))
            .set_drawing_area(DrawingArea(200,200))
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

   add_background_component(
            &(* new Label())
            .set_label("About")
            .set_border_size(0)
            .set_align_right()
            .set_padding(30)
            .set_theme_style(Theme::style_brand_primary)
            .set_drawing_point(DrawingPoint(200,0))
            .set_drawing_area(DrawingArea(800,200))
            );

   add_background_component(
            &(* new Label())
            .set_label("Version")
            .set_border_size(0)
            .set_align_left()
            .set_theme_style(Theme::style_light)
            .set_drawing_point(DrawingPoint(20,200))
            .set_drawing_area(DrawingArea(980,100))
            );

   add_background_component(
            &(* new Label())
            .set_label(info.bsp_version() + "." + info.kernel_version())
            .set_border_size(0)
            .set_align_left()
            .set_theme_style(Theme::style_outline_brand_primary)
            .set_drawing_point(DrawingPoint(20,300))
            .set_drawing_area(DrawingArea(980,150))
            );








}



