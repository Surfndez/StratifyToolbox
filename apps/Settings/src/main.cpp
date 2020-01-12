
#include <sapi/sys.hpp>
#include "Application.hpp"

int main(int argc, char * argv[]){
   Cli cli(argc, argv, SOS_GIT_HASH);
   Application application(cli);
   application.run("Display");
   return 0;
}
