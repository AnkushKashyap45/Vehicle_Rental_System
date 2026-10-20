#ifndef trip_h
#define trip_h

/* Include custom headers */

#include "date.h"
#include "storable.cpp"
#include "user.h"
#include "vehicle.h"

/* Include standard cpp headers */

#include <string>

using namespace std;

/* Trip class which helps to manage trip-related data */

class Trip : public Storable {

private:

    /* Pointer to constant vehicle in memory */
    /* We will create vehicle in memory and assign address of that vehicle to 'vehicle' */
    /* The pointed-to Vehicle cannot be modified through this pointer */

    const Vehicle * vehicle;

    /* Pointer to constant user in memory */
    /* We will create user in memory and assign address of that user to 'user' */

    const User * user;

    /* We need to track record of start and end date of the trip so that at the time of booking
       a trip we can find vehicles that are free in that specific duration */

    /* Start date of trip */
    Date startDate;

    /* End date of trip */
    Date endDate;

    /* We also need to keep track of reading of Odometer so that fare can be calculated */

    long startReading;
    long endReading;

    /* Fare of this trip */
    double fare;

    /* Trip is completed or not */
    bool completed;

public:

    /*
        Constructor
        input : Pointer to vehicle in memory
        input : Pointer to user in memory
        input : Start date of trip
        input : End date of trip
        input : recordId (defaults to 0)
        input : start reading, end reading (default 0)
        input : fare (default 0.0), isCompleted (default false)
    */

    Trip ( const Vehicle * vehicle, const User * user, Date startDate, Date endDate, long recordId = 0, long startReading = 0, long endReading = 0, double fare = 0.0, bool isCompleted = false );

    /*
        Getter methods with const attribute i.e. this method is not changing any data of the current object
    */

    const User & getUser ( ) const;

    const Vehicle & getVehicle ( ) const;

    Date getStartDate ( ) const;

    Date getEndDate ( ) const;

    long getStartReading ( ) const;

    long getEndReading ( ) const;

    double getFare ( ) const;

    bool isCompleted ( ) const;

    /*
        Start trip
        input: Start reading of vehicle's odometer
    */

    void startTrip ( long startReading );

    /*
        Complete trip
        input: End reading of vehicle's odometer
        output: calculate and return fare of trip
    */

    double completeTrip ( long endReading );

    /* Display record on screen */

    void display ( ) const;

    /* Overridden virtual method */

    string toString ( ) const;

    void setDataFrom ( Storable * s );

};

#endif
