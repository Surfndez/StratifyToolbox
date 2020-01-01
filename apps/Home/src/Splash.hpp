#ifndef SPLASH_HPP
#define SPLASH_HPP

#include <ToolboxAPI/Application.hpp>

class Application;

class Splash :
      public toolbox::ApplicationScene<Application> {
public:
   Splash(Application & application);


};

#endif // SPLASH_HPP
