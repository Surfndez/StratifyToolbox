#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;


class Display : public toolbox::ApplicationLayout<Application> {
public:
   Display(Application & application);



private:


};

#endif // DISPLAY_HPP
