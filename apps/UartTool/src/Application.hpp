#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ToolboxAPI/toolbox.hpp>
#include <sapi/sys/Cli.hpp>
#include <sapi/ux.hpp>

class Application : public ToolboxApplication {

public:
   Application(const sys::Cli & cli);

	 ux::Layout& create_layout();

private:


};

#endif // APPLICATION_HPP
