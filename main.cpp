/*************************************************
               VEHICLE RENTAL SYSTEM
     COMPILED ON LINUX, MAC AND WINDOWS (C++11 or newer)
*************************************************/

/* Include custom headers */

#include "application.h"

/* Include standard cpp headers */

#include <iostream>

using namespace std;

int main ( int argc, const char * argv[] ) {

    Application * application = new Application();
    application->start();

    return 0;
}
