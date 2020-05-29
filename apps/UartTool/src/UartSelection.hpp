#ifndef UARTSELECTION_HPP
#define UARTSELECTION_HPP


#include <ToolboxAPI/components.hpp>

class Application;

class UartSelection : public ApplicationLayout<UartSelection>{
public:
	UartSelection(Application * app);

	static const char * top_navigation_name(){
		return "UartSelectionTopNavigation";
	}

	void local_event_handler(const ux::Event & event);

private:
	API_ACCESS_COMPOUND(UartSelection,var::String,caller);

};

#endif // UARTSELECTION_HPP
