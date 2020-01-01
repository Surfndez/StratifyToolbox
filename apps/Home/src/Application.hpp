#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ToolboxAPI/Application.hpp>
#include <sapi/sys/Cli.hpp>

#include "Home.hpp"
#include "Launcher.hpp"
#include "Splash.hpp"

class Application : public toolbox::Application {

public:
   Application(const sys::Cli & cli);


private:


};

#endif // APPLICATION_HPP
