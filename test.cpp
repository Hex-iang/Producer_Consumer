#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;
bool done = true;
const int ESCAPE=27;
const int RETURN=10;
// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void * signal_callback_handler(void * args)
{
   while (true) {
	   char ch;
	   cin.get(ch);
	   if(ch == RETURN){
	   	printf("Return......");
	   	done = false;
	   }
	   if(ch == ESCAPE){
	   	printf("Exit......\n");
	   	done = false;
	   }
   }
}

int main()
{
   // Register signal and signal handler
   pthread_t thread;
   pthread_create(&thread, NULL, signal_callback_handler,NULL);

   while(done)
   {
	  printf("Program processing stuff here.\n");
	  sleep(1);
   }
   return EXIT_SUCCESS;
}

