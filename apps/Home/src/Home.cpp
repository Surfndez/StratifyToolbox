#include "Home.hpp"
#include "Application.hpp"

#include <sapi/ux/ProgressBar.hpp>
#include <sapi/ux/ToggleSwitch.hpp>

Home::Home(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(500);
   add_component(
            "ToggleSwitch",
            &(* new ux::ToggleSwitch())
            .set_state(true)
            .set_drawing_point(ux::DrawingPoint(600,600))
            .set_drawing_area(ux::DrawingArea(200,200*1.3f))
            .set_context(this)
            .set_event_handler(
               [&](void * context, const ux::Event & event){
      application.printer().info("Handle event");
   })

            );


}



