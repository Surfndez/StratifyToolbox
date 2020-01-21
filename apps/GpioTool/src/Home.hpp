#ifndef HOME_HPP
#define HOME_HPP

#include <sapi/var/Vector.hpp>
#include <sapi/ux/Scene.hpp>
#include <ToolboxAPI/Application.hpp>

class Application;


class Home : public toolbox::ApplicationScene<Application> {
public:
   Home(Application & application);
   //~Home();



private:


};

#endif // HOME_HPP
