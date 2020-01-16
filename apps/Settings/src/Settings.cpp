#include "Settings.hpp"
#include "Application.hpp"

#include <sapi/ux.hpp>

Settings::Settings(Application & application)
   : toolbox::ApplicationScene<Application>(application){

   update_period() = chrono::Milliseconds(10);
   add_component(
            "List",
            &(* new List())
            .append(ListItem()
                    .set_key("Account")
                    .set_value("tgil")
                    .set_icon("chevron-right")
                    )
            .append(ListItem()
                    .set_key("Display")
                    .set_icon("chevron-right")
                    )
            .append(ListItem()
                    .set_key("Network")
                    .set_icon("chevron-right")
                    )
            .append(ListItem()
                    .set_key("About")
                    .set_icon("chevron-right")
                    )
            .set_item_height(250)
            .set_drawing_point(DrawingPoint(0,0))
            .set_drawing_area(DrawingArea(1000,1000))
            .set_theme_style(Theme::style_dark)
            .set_event_handler(
               [&](Component * object, const Event & event){

               if( event.type() == ListEvent::event_type() ){
                  const ListItem & item = event.reinterpret<ListEvent>().item();
                  application.printer().info("Item is %s",
                  item.key().cstring()
                  );

                  if( item.key() == "Display" ){
                     application.printer().info("set scene collection to Display");
                     scene_collection()->set_current_scene("Display");
                  } else if( item.key() == "About" ){
                     application.printer().info("set scene collection to Display");
                     scene_collection()->set_current_scene("About");
                  }
               }

            })

         );





}



