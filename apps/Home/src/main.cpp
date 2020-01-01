
#include <sapi/sys.hpp>
#include "Application.hpp"

int main(int argc, char * argv[]){
   Cli cli(argc, argv, SOS_GIT_HASH);

   printf("Construct application\n");
   Application application(cli);
   printf("run appliation\n");
   application.run("home");

   return 0;
}
