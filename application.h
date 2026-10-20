#ifndef application_h
#define application_h

/* Include custom headers */

#include "vehicle.h"
#include "user.h"
#include "trip.h"
#include "database.h"
#include "error.cpp"
#include "string_helper.h"

/* Include standard cpp headers */

#include <fstream>
#include <string>

class Application {

private:

    Database * db;

    void renderMenu ( );
    void welcome ( );
    void gotoXY ( int x, int y ) const;
    void clearScreen ( ) const;

    /* Render menus */

    void renderAddNewVehicleMenu ( ) const;
    void renderViewVehicleMenu ( ) const;
    void renderEditVehicleMenu ( ) const;
    void renderAddNewUserMenu ( ) const;
    void renderAddNewTripMenu ( ) const;
    void renderViewTripMenu ( ) const;
    void renderStartTripMenu ( ) const;
    void renderCompleteTripMenu ( ) const;
    void showDialog ( string message, string id = "" ) const;

    void cleanMemory ( );

public:

    Application();

    /* start application */
    void start();
};

#endif
