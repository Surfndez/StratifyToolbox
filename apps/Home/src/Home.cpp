#include "Home.hpp"
#include "Application.hpp"

#include <sapi/ux/ProgressBar.hpp>
#include <sapi/ux/ToggleSwitch.hpp>

Home::Home(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);
   add_component(
            "ToggleSwitch",
            &(* new ux::ToggleSwitch())
            .set_state(true)
            .set_drawing_point(ux::DrawingPoint(600,600))
            .set_drawing_area(ux::DrawingArea(200,200*1.3f))
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


}



