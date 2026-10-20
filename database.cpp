
/* Include custom headers */

#include "database.h"
#include "string_helper.h"

/* Include standard cpp headers */

#include <iostream>
#include <fstream>
#include <new>

using namespace std;

const char DELIMITER = ';' ;

/* Make sure a data file exists. If it does not, create an empty one. */
/* (The book expects you to create the three .txt files by hand.) */

static void ensureFileExists ( const string & name ) {
    ofstream file ( name.c_str(), ios::app );
}

Database :: Database ( ) {

    try {

        ensureFileExists ( "vehicles.txt" );
        ensureFileExists ( "users.txt" );
        ensureFileExists ( "trips.txt" );

        this->vehicleTable = new Table<Vehicle> ( "vehicles.txt" );
        this->userTable = new Table<User> ( "users.txt" );
        this->tripTable = new Table<Trip> ( "trips.txt" );

        /* Fetch data for vehicles */
        this->fetchAllVehicles();

        /* Fetch data for users */
        this->fetchAllUsers();

        /* Fetch data for trips (must be last: trips point to vehicles and users) */
        this->fetchAllTrips();
    }
    catch ( ... ) {
        this->cleanUp();
        throw;
    }
}

void Database :: fetchAllVehicles ( ) {

    /* Open file for reading */
    this->vehicleTable->fileStream.clear();
    this->vehicleTable->fileStream.open ( this->vehicleTable->fileName, ios::in );

    if ( !this->vehicleTable->fileStream ) {
        throw IOError();
    }

    /* Read all lines from file */
    string line;

    while ( getline ( this->vehicleTable->fileStream, line ) ) {

        if ( line.empty() ) {
            continue;
        }

        /* Split string with given delimiter */
        vector<string> components = split ( line, DELIMITER );

        if ( components.size() < 7 ) {
            continue;   /* damaged line, skip it */
        }

        try {

            /* Get all components from vector */
            auto recordId = stol ( components[0] );
            auto registrationNumber = components[1];
            auto type = stoi ( components[2] );
            auto seats = stoi ( components[3] );
            auto companyName = components[4];
            auto pricePerKm = stod ( components[5] );
            auto PUCExpirationDate = components[6];

            /* Create record and store */
            Storable * record = new Vehicle ( registrationNumber, VehicleType ( type ), seats, companyName, pricePerKm, Date ( PUCExpirationDate ), recordId );

            this->vehicleTable->records->push_back ( record );
        }
        catch ( const bad_alloc & ) {
            this->vehicleTable->fileStream.close();
            throw MemoryError();
        }
        catch ( const exception & ) {
            /* number conversion failed: damaged line, skip it */
        }
    }

    /* Close file */
    this->vehicleTable->fileStream.close();
}

void Database :: fetchAllUsers ( ) {

    /* Open file for reading */
    this->userTable->fileStream.clear();
    this->userTable->fileStream.open ( this->userTable->fileName, ios::in );

    if ( !this->userTable->fileStream ) {
        throw IOError();
    }

    /* Read all lines from file */
    string line;

    while ( getline ( this->userTable->fileStream, line ) ) {

        if ( line.empty() ) {
            continue;
        }

        /* Split string with given delimiter */
        vector<string> components = split ( line, DELIMITER );

        if ( components.size() < 4 ) {
            continue;
        }

        try {

            /* Get all components from vector */
            auto recordId = stol ( components[0] );
            auto name = components[1];
            auto contact = components[2];
            auto email = components[3];

            /* Create record and store */
            Storable * record = new User ( name, contact, email, recordId );

            this->userTable->records->push_back ( record );
        }
        catch ( const bad_alloc & ) {
            this->userTable->fileStream.close();
            throw MemoryError();
        }
        catch ( const exception & ) {
            /* damaged line, skip it */
        }
    }

    /* Close file */
    this->userTable->fileStream.close();
}

void Database :: fetchAllTrips ( ) {

    /* Open file for reading */
    this->tripTable->fileStream.clear();
    this->tripTable->fileStream.open ( this->tripTable->fileName, ios::in );

    if ( !this->tripTable->fileStream ) {
        throw IOError();
    }

    /* Read all lines from file */
    string line;

    while ( getline ( this->tripTable->fileStream, line ) ) {

        if ( line.empty() ) {
            continue;
        }

        /* Split string with given delimiter */
        vector<string> components = split ( line, DELIMITER );

        if ( components.size() < 9 ) {
            continue;
        }

        try {

            /* Get all components from vector */
            auto recordId = stol ( components[0] );
            auto vehicleId = stol ( components[1] );
            auto userId = stol ( components[2] );
            auto startDate = Date ( components[3] );
            auto endDate = Date ( components[4] );
            auto startReading = stol ( components[5] );
            auto endReading = stol ( components[6] );
            auto fare = stod ( components[7] );
            auto isCompleted = ( components[8] == "1" ) ? true : false;

            /* A trip only stores the IDs in the file. Find the real Vehicle and User objects in memory */
            const Vehicle * vehicle = this->vehicleTable->getReferenceOfRecordForId ( vehicleId );
            const User * user = this->userTable->getReferenceOfRecordForId ( userId );

            /* Create record and store */
            Storable * record = new Trip ( vehicle, user, startDate, endDate, recordId, startReading, endReading, fare, isCompleted );

            this->tripTable->records->push_back ( record );
        }
        catch ( const bad_alloc & ) {
            this->tripTable->fileStream.close();
            throw MemoryError();
        }
        catch ( const NoSuchRecordError & ) {
            /* trip refers to a vehicle/user that no longer exists: ignore this line */
        }
        catch ( const exception & ) {
            /* damaged line, skip it */
        }
    }

    /* Close file */
    this->tripTable->fileStream.close();
}

const Vehicle * const Database :: getVehicle ( string registrationNo ) const {

    for ( auto & record : *this->vehicleTable->records ) {

        Vehicle * vehicle = dynamic_cast<Vehicle*> ( record );

        if ( vehicle && vehicle->getRegistrationNumber() == registrationNo ) {
            return vehicle;
        }
    }

    throw NoSuchRecordError();
}

const User * const Database :: getUser ( string contactNo ) const {

    for ( auto & record : *this->userTable->records ) {

        User * user = dynamic_cast<User*> ( record );

        if ( user && user->getContact() == contactNo ) {
            return user;
        }
    }

    throw NoSuchRecordError();
}

const vector<const Vehicle *> Database :: getVehicle ( Date startDate, Date endDate, VehicleType type ) const {

    vector<const Vehicle *> vehicles = vector<const Vehicle *>();

    for ( auto & vrecord : *this->vehicleTable->records ) {

        Vehicle * vehicle = dynamic_cast<Vehicle*> ( vrecord );

        if ( vehicle && vehicle->getVehicleType() == type ) {

            /* Check if this vehicle is free between startDate and endDate */
            bool tripFound = false;

            for ( auto & trecord : *this->tripTable->records ) {

                Trip * trip = dynamic_cast<Trip*> ( trecord );

                if (
                    trip &&
                    !trip->isCompleted() &&
                    trip->getVehicle().getRecordId() == vehicle->getRecordId() &&
                    !( trip->getStartDate() >= endDate && trip->getEndDate() >= endDate ) &&
                    !( trip->getStartDate() <= startDate && trip->getEndDate() <= startDate )
                ) {
                    tripFound = true;
                    break;
                }
            }

            if ( !tripFound ) {
                vehicles.push_back ( vehicle );
            }
        }
    }

    return vehicles;
}

void Database :: cleanUp ( ) {
    delete this->vehicleTable;
    delete this->userTable;
    delete this->tripTable;

    this->vehicleTable = NULL;
    this->userTable = NULL;
    this->tripTable = NULL;
}

Database :: ~Database ( ) {
    this->cleanUp();
}

const Table<Vehicle> * const Database :: getVehicleRef ( ) const {
    return this->vehicleTable;
}

const Table<User> * const Database :: getUserRef ( ) const {
    return this->userTable;
}

const Table<Trip> * const Database :: getTripRef ( ) const {
    return this->tripTable;
}
