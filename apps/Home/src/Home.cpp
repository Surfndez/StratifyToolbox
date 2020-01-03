#include "Home.hpp"
#include "Application.hpp"

#include <sapi/ux/Button.hpp>
#include <sapi/ux/ProgressBar.hpp>
#include <sapi/ux/ToggleSwitch.hpp>

Home::Home(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);
   add_component(
            "ToggleSwitch",
            &(* new ux::ToggleSwitch())
            .set_state(true)
            .set_drawing_point(ux::DrawingPoint(800,50))
            .set_drawing_area(ux::DrawingArea(200,150))
            .set_event_handler(
               [&](ux::Component * object, const ux::Event & event){
               //ux::ToggleSwitch * toggle_switch =
               //static_cast<ux::ToggleSwitch *>(object);

               if( event.type() == ux::ToggleSwitchEvent::event_type() ){
                  application.printer().info("New state is %d",
                  event.reinterpret<ux::ToggleSwitchEvent>().state()
                  );
               }

   })

            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("1")
            .set_theme_style(Theme::style_brand_primary)
            .set_drawing_point(ux::DrawingPoint(0,0))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("2")
            .set_theme_style(Theme::style_brand_secondary)
            .set_drawing_point(ux::DrawingPoint(350,0))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("light")
            .set_theme_style(Theme::style_light)
            .set_drawing_point(ux::DrawingPoint(0,350))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("info")
            .set_theme_style(Theme::style_info)
            .set_drawing_point(ux::DrawingPoint(350,350))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("success")
            .set_theme_style(Theme::style_success)
            .set_drawing_point(ux::DrawingPoint(0,350))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("danger")
            .set_theme_style(Theme::style_danger)
            .set_drawing_point(ux::DrawingPoint(0,700))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Button",
            &(* new ux::Button())
            .set_label("warning")
            .set_theme_style(Theme::style_warning)
            .set_drawing_point(ux::DrawingPoint(350,700))
            .set_drawing_area(ux::DrawingArea(300,300))
            );


}



