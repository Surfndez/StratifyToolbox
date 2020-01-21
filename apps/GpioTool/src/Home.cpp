#include "Home.hpp"
#include "Application.hpp"

#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/ux.hpp>

Home::Home(Application & application)
	: toolbox::ApplicationScene<Application>(application){

	update_period() = chrono::Milliseconds(10);






}



