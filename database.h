#ifndef database_h
#define database_h

/* Include custom headers */

#include "vehicle.h"
#include "user.h"
#include "trip.h"
#include "table.cpp"
#include "error.cpp"

using namespace std;

/*
    This is a Database class and acts as a middleware, so we can hide low-level details like file handling
    add, update records, etc.
*/

class Database {

private:

    /* Database has multiple tables */

    /* Vehicle table */
    Table<Vehicle> * vehicleTable = NULL;

    /* User table */
    Table<User> * userTable = NULL;

    /* Trip table */
    Table<Trip> * tripTable = NULL;

    /*
        Fetch all data from files
        Can throw 'IOError' or 'MemoryError'
    */

    void fetchAllVehicles();
    void fetchAllUsers();
    void fetchAllTrips();

    void cleanUp();

public:

    /*
        Constructor can throw 'MemoryError' or 'IOError'
        If the system is out of memory, or I/O Operation cannot be performed, then this error will be thrown
    */

    Database();

    ~Database();

    /* Get references to database tables */
    /* All are constant pointers to constant objects. So, user can neither change pointer nor the data */

    const Table<Vehicle> * const getVehicleRef() const;

    const Table<User> * const getUserRef() const;

    const Table<Trip> * const getTripRef() const;

    /* Query vehicle by 'registrationNo' */

    const Vehicle * const getVehicle ( string registrationNo ) const;

    /* Query user by contact no. */

    const User * const getUser ( string contactNo ) const;

    /* Find vehicles of given type, which are available in a given duration */

    const vector<const Vehicle *> getVehicle ( Date startDate, Date endDate, VehicleType type ) const;

    /* Add new record of type T in database */

    template <class T> void addNewRecord ( T * record );

    /* Update record in database */

    template <class T> void updateRecord ( T * record );

};

#endif
