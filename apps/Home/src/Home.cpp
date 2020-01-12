#include "Home.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>

Home::Home(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);
   add_component(
            "ToggleSwitch",
            &(* new ToggleSwitch())
            .set_state(true)
            .set_drawing_point(DrawingPoint(20,800))
            .set_drawing_area(ux::DrawingArea(200,150))
            .set_event_handler(
               [&](Component * object, const ux::Event & event){

               if( event.type() == ux::ToggleSwitchEvent::event_type() ){
                  application.printer().info("New state is %d",
                  event.reinterpret<ux::ToggleSwitchEvent>().state()
                  );
               }

            })

         );

   const ux::DrawingArea control_icon_area(200,250);
   const ux::DrawingPoint control_icon_start(800,0);
   const drawing_size_t control_icon_spacing = 250;
   const u8 control_border_size = 0;
   const enum Theme::style control_icon_style = Theme::style_dark;

   add_component(
            "WifiButton",
            &(* new ux::Button())
            .set_border_size(control_border_size)
            .set_icon_name("wifi")
            .set_theme_style(control_icon_style)
            .set_drawing_point(control_icon_start)
            .set_drawing_area(control_icon_area)
            );

   add_component(
            "LaunchButton",
            &(* new ux::Button())
            .set_border_size(control_border_size)
            .set_icon_name("rocket")
            .set_theme_style(control_icon_style)
            .set_drawing_point(control_icon_start + ux::DrawingPoint::Y(control_icon_spacing))
            .set_drawing_area(control_icon_area)
            );

   add_component(
            "EjectButton",
            &(* new ux::Button())
            .set_border_size(control_border_size)
            .set_icon_name("eject")
            .set_theme_style(control_icon_style)
            .set_drawing_point(control_icon_start + ux::DrawingPoint::Y(2*control_icon_spacing))
            .set_drawing_area(control_icon_area)
            );

   add_component(
            "SettingsButton",
            &(* new ux::Button())
            .set_border_size(control_border_size)
            .set_icon_name("wrench")
            .set_theme_style(control_icon_style)
            .set_drawing_point(control_icon_start + ux::DrawingPoint::Y(3*control_icon_spacing))
            .set_drawing_area(control_icon_area)
            );

   add_component(
            "Button1",
            &(* new ux::Button())
            .set_label("1")
            .set_theme_style(Theme::style_brand_primary)
            .set_drawing_point(ux::DrawingPoint(50,50))
            .set_drawing_area(ux::DrawingArea(200,200))
            );

   add_component(
            "Button2",
            &(* new ux::Button())
            .set_border_size(5)
            .set_icon_name("ban")
            .set_theme_style(Theme::style_light)
            .set_drawing_point(ux::DrawingPoint(350,50))
            .set_drawing_area(ux::DrawingArea(300,300))
            );

   add_component(
            "Slider1",
            &(* new ux::Slider())
            .set_value(25)
            .set_maximum(100)
            .set_theme_style(Theme::style_danger)
            .set_drawing_point(ux::DrawingPoint(100,400))
            .set_drawing_area(ux::DrawingArea(400,100))
            .set_antialias(false)
            .set_event_handler(
               [&](ux::Component * object, const ux::Event & event){

               if( event.type() == ux::SliderEvent::event_type() ){
                  ux::ProgressBar * progress_bar = find_component<ux::ProgressBar>("Progress1");
                  if( progress_bar ){
                     application.printer().info("update progress bar %d/%d",
                     event.reinterpret<ux::SliderEvent>().value(),
                     event.reinterpret<ux::SliderEvent>().maximum()
                     );
                     progress_bar->set_value( event.reinterpret<ux::SliderEvent>().value() );
                     progress_bar->set_maximum( event.reinterpret<ux::SliderEvent>().maximum() );
                     progress_bar->redraw();
                  }
               }

            })
         );

   add_component(
            "Progress1",
            &(* new ux::ProgressBar())
            .set_border_thickness(50)
            .set_value(25)
            .set_maximum(100)
            .set_theme_style(Theme::style_brand_secondary)
            .set_drawing_point(ux::DrawingPoint(100,550))
            .set_drawing_area(ux::DrawingArea(400,50))
            );



}



