#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <sapi/ui.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;

class Launcher :
      public toolbox::ApplicationScene<Application> {
public:
   Launcher(Application & application);

};

#endif // LAUNCHER_HPP
