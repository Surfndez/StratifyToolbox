#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ToolboxAPI/ToolboxApplication.hpp>
#include <sapi/sys/Cli.hpp>

class Application : public toolbox::ToolboxApplication {

public:
   Application(const sys::Cli & cli);

	 ux::Layout& create_layout();

private:


};

#endif // APPLICATION_HPP
