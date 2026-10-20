/* Windows only: needed to switch on ANSI escape codes (used by gotoXY). Must come first. */
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* Include custom headers */

#include "application.h"
#include "database_templates.cpp"

/* Include standard cpp headers */

#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

/* ---------- small helper functions (only visible inside this file) ---------- */

/* Windows consoles need a flag set before they understand ANSI escape codes */
static void enableAnsiEscapeCodes ( ) {
#ifdef _WIN32
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    if ( h == INVALID_HANDLE_VALUE ) return;
    DWORD mode = 0;
    if ( !GetConsoleMode ( h, &mode ) ) return;
    SetConsoleMode ( h, mode | 0x0004 );   /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */
#endif
}

/* Read one number and throw away the rest of the line. Bad input becomes 0 instead of breaking cin. */
template <class T> static void readValue ( T & value ) {
    if ( !( cin >> value ) ) {
        cin.clear();
        value = T();
    }
    cin.ignore ( numeric_limits<streamsize>::max(), '\n' );
}

/* ';' is our file delimiter, so it must never appear inside the data */
static string clean ( string text ) {
    for ( size_t i = 0; i < text.length(); i++ ) {
        if ( text[i] == ';' ) text[i] = ',';
    }
    return text;
}

/* ---------- Application ---------- */

Application :: Application ( ) {

    enableAnsiEscapeCodes();
    this->db = NULL;

    try {
        this->db = new Database();
    }
    catch ( const Error & e ) {
        cout << e.getMessage() << endl;
        exit ( EXIT_FAILURE );
    }
}

/* Move the cursor to column x, row y using an ANSI escape code */
void Application :: gotoXY ( int x, int y ) const {
    printf ( "%c[%d;%df", 0x1B, y, x );
    fflush ( stdout );
}

/* Clear the screen and move the cursor to the top-left corner */
void Application :: clearScreen ( ) const {
    printf ( "%c[2J%c[H", 0x1B, 0x1B );
    fflush ( stdout );
}

void Application :: renderMenu ( ) {

    char choice = '0';

    while ( true ) {

        this->clearScreen();

        gotoXY ( 25, 4 );
        cout << "Select Any option from below";

        gotoXY ( 25, 5 );
        cout << "1. Add new vehicle";
        gotoXY ( 25, 6 );
        cout << "2. View vehicle details";
        gotoXY ( 25, 7 );
        cout << "3. Edit vehicle details";
        gotoXY ( 25, 8 );
        cout << "4. Add new user";
        gotoXY ( 25, 9 );
        cout << "5. Add new trip";
        gotoXY ( 25, 10 );
        cout << "6. View trip";
        gotoXY ( 25, 11 );
        cout << "7. Start trip";
        gotoXY ( 25, 12 );
        cout << "8. Complete trip";
        gotoXY ( 25, 13 );
        cout << "9. Exit";

        gotoXY ( 30, 15 );
        printf ( "Enter your choice : " );
        fflush ( stdout );

        gotoXY ( 50, 15 );

        if ( !( cin >> choice ) ) {
            /* input stream is closed (Ctrl+D / Ctrl+Z): leave the program */
            this->cleanMemory();
            exit ( EXIT_SUCCESS );
        }
        cin.ignore ( numeric_limits<streamsize>::max(), '\n' );

        switch ( choice ) {
            case '1':
                this->renderAddNewVehicleMenu();
                break;
            case '2':
                this->renderViewVehicleMenu();
                break;
            case '3':
                this->renderEditVehicleMenu();
                break;
            case '4':
                this->renderAddNewUserMenu();
                break;
            case '5':
                this->renderAddNewTripMenu();
                break;
            case '6':
                this->renderViewTripMenu();
                break;
            case '7':
                this->renderStartTripMenu();
                break;
            case '8':
                this->renderCompleteTripMenu();
                break;
            case '9':
                this->cleanMemory();
                this->clearScreen();
                exit ( EXIT_SUCCESS );
                break;
            default:
                this->showDialog ( "Invalid option. Please enter a number from 1 to 9" );
                break;
        }
    }
}

void Application :: renderAddNewVehicleMenu ( ) const {

    string header = "Enter details of vehicle (All fields are required) ";
    string registrationNoLabel  = "Enter vehicle registration number :  ";
    string vehicleTypeLabel = "Enter vehicle type :  ";
    string vehicleOptionLabel = "(1. Bike, 2. Car, 3. Towera)";
    string seatLabel = "Enter no. of seats :  ";
    string companyNameLabel = "Enter name of company :  ";
    string priceLabel = "Enter price per km :  ";
    string pucExpirationDateLabel = "Enter PUC expiration date (d/m/yyyy) :  ";

    string registrationNo;
    int vehicleType = 0;
    int seat = 0;
    string companyName;
    double price = 0;
    string pucExpirationDate;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( 0, 3 );
    cout << registrationNoLabel;

    gotoXY ( 0, 4 );
    cout << vehicleTypeLabel;
    gotoXY ( 0, 5 );
    cout << vehicleOptionLabel;

    gotoXY ( 0, 6 );
    cout << seatLabel;

    gotoXY ( 0, 7 );
    cout << companyNameLabel;

    gotoXY ( 0, 8 );
    cout << priceLabel;

    gotoXY ( 0, 9 );
    cout << pucExpirationDateLabel;

    /* Prepare input fields */

    gotoXY ( int ( registrationNoLabel.length() ), 3 );
    getline ( cin, registrationNo );

    gotoXY ( int ( vehicleTypeLabel.length() ), 4 );
    readValue ( vehicleType );

    gotoXY ( int ( seatLabel.length() ), 6 );
    readValue ( seat );

    gotoXY ( int ( companyNameLabel.length() ), 7 );
    getline ( cin, companyName );

    gotoXY ( int ( priceLabel.length() ), 8 );
    readValue ( price );

    gotoXY ( int ( pucExpirationDateLabel.length() ), 9 );
    getline ( cin, pucExpirationDate );

    /* ---- validate what the user typed ---- */

    registrationNo = clean ( registrationNo );
    companyName = clean ( companyName );

    if ( registrationNo.empty() || companyName.empty() ) {
        this->showDialog ( "Registration number and company name are required" );
        return;
    }

    if ( vehicleType < 1 || vehicleType > 3 ) {
        this->showDialog ( "Vehicle type must be 1, 2 or 3" );
        return;
    }

    if ( seat <= 0 || price < 0 ) {
        this->showDialog ( "Seats must be above 0 and price cannot be negative" );
        return;
    }

    if ( Date ( pucExpirationDate ).isEmpty() ) {
        this->showDialog ( "Invalid date. Please use d/m/yyyy format, e.g. 5/12/2026" );
        return;
    }

    /* Do not allow two vehicles with the same registration number */
    try {
        this->db->getVehicle ( registrationNo );
        this->showDialog ( "A vehicle with this registration number already exists" );
        return;
    }
    catch ( const NoSuchRecordError & e ) {
        /* good: nothing found, so the number is new */
    }

    Vehicle * vehicle = NULL;

    try {
        vehicle = new Vehicle ( registrationNo, VehicleType ( vehicleType ), seat, companyName, price, Date ( pucExpirationDate ) );
        this->db->addNewRecord ( vehicle );

        stringstream ss;
        ss << "Vehicle Id : " << vehicle->getRecordId();
        showDialog ( "Vehicle added successfully", ss.str() );
    }
    catch ( const Error & e ) {
        showDialog ( e.getMessage() );
    }

    delete vehicle;
}

void Application :: renderViewVehicleMenu ( ) const {

    string header = "Enter registration no. of vehicle :  ";
    string registrationNo;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( int ( header.length() ), 1 );
    getline ( cin, registrationNo );

    /* Find vehicle and display data */
    gotoXY ( 0, 3 );

    try {
        auto vehicle = this->db->getVehicle ( registrationNo );
        vehicle->display();

        cout << endl << "Press any key to continue";
        cin.get();
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
    }
}

void Application :: renderEditVehicleMenu ( ) const {

    string header = "Enter registration no. of vehicle you want to edit :  ";
    string priceLabel = "Enter new price per km :  ";

    string registrationNo;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( int ( header.length() ), 1 );
    getline ( cin, registrationNo );

    /* Find vehicle and display data */
    gotoXY ( 0, 3 );

    Vehicle * modifiedVehicle = NULL;

    try {
        auto vehicle = this->db->getVehicle ( registrationNo );

        /* work on a COPY; the real record changes only when updateRecord succeeds */
        modifiedVehicle = new Vehicle ( *vehicle );
        vehicle->display();

        string newPrice;

        gotoXY ( 0, 12 );
        cout << priceLabel;

        gotoXY ( int ( priceLabel.length() ), 12 );
        getline ( cin, newPrice );

        if ( newPrice != "" ) {
            modifiedVehicle->setPricePerKm ( stod ( newPrice ) );
            this->db->updateRecord ( modifiedVehicle );

            this->showDialog ( "Vehicle data updated successfully" );
        }
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
    }
    catch ( const exception & e ) {
        /* stod throws when the text is not a number */
        this->showDialog ( "Invalid price. Please enter a number" );
    }

    delete modifiedVehicle;
}

void Application :: renderAddNewUserMenu ( ) const {

    string header = "Enter details of user ";
    string userNameLabel  = "Enter name of user :  ";
    string usercontactLabel = "Enter contact no of user :  ";
    string userEmailLabel = "Enter email of user (optional) :  ";

    string name;
    string contact;
    string email;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( 0, 3 );
    cout << userNameLabel;

    gotoXY ( 0, 4 );
    cout << usercontactLabel;

    gotoXY ( 0, 5 );
    cout << userEmailLabel;

    /* Prepare input fields */

    gotoXY ( int ( userNameLabel.length() ), 3 );
    getline ( cin, name );

    gotoXY ( int ( usercontactLabel.length() ), 4 );
    getline ( cin, contact );

    gotoXY ( int ( userEmailLabel.length() ), 5 );
    getline ( cin, email );

    name = clean ( name );
    contact = clean ( contact );
    email = clean ( email );

    if ( name.empty() || contact.empty() ) {
        this->showDialog ( "Name and contact number are required" );
        return;
    }

    /* Contact number is used to find users, so it must be unique */
    try {
        this->db->getUser ( contact );
        this->showDialog ( "A user with this contact number already exists" );
        return;
    }
    catch ( const NoSuchRecordError & e ) {
        /* good: nothing found, so the number is new */
    }

    User * user = NULL;

    try {
        user = new User ( name, contact, email );
        this->db->addNewRecord ( user );

        stringstream ss;
        ss << "User Id : " << user->getRecordId();
        showDialog ( "User added successfully", ss.str() );
    }
    catch ( const Error & e ) {
        showDialog ( e.getMessage() );
    }

    delete user;
}

void Application :: renderAddNewTripMenu ( ) const {

    string header = "Enter details of trip (All fields are required) ";

    string userContactLabel = "Enter contact no of user :  ";
    string startDateLabel = "Enter start date of trip (d/m/yyyy) :  ";
    string endDateLabel = "Enter end date of trip (d/m/yyyy) :  ";
    string vehicleTypeLabel = "Enter vehicle type :  ";
    string vehicleOptionLabel = "(1. Bike, 2. Car, 3. Towera)";
    string registrationNoLabel = "Enter registration no of vehicle :  ";

    string contactNo;
    string startDate;
    string endDate;
    int vehicleType = 0;

    string registrationNo;

    const User * user = NULL;
    const Vehicle * vehicle = NULL;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << userContactLabel;

    gotoXY ( int ( userContactLabel.length() ), 1 );
    getline ( cin, contactNo );

    try {
        user = this->db->getUser ( contactNo );
        gotoXY ( 0, 3 );
        cout << "User found : " << user->getName();
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
        return;
    }

    gotoXY ( 0, 5 );
    cout << header;

    gotoXY ( 0, 7 );
    cout << startDateLabel;

    gotoXY ( 0, 8 );
    cout << endDateLabel;

    gotoXY ( 0, 9 );
    cout << vehicleTypeLabel;
    gotoXY ( 0, 10 );
    cout << vehicleOptionLabel;

    gotoXY ( int ( startDateLabel.length() ), 7 );
    getline ( cin, startDate );

    gotoXY ( int ( endDateLabel.length() ), 8 );
    getline ( cin, endDate );

    gotoXY ( int ( vehicleTypeLabel.length() ), 9 );
    readValue ( vehicleType );

    /* ---- validate dates ---- */

    if ( Date ( startDate ).isEmpty() || Date ( endDate ).isEmpty() ) {
        this->showDialog ( "Invalid date. Please use d/m/yyyy format, e.g. 5/12/2026" );
        return;
    }

    if ( Date ( endDate ) < Date ( startDate ) ) {
        this->showDialog ( "End date cannot be before start date" );
        return;
    }

    /* Find vehicle as per user's requirements, and if not found, ask the user to change the input */

    auto freeVehicles = this->db->getVehicle ( Date ( startDate ), Date ( endDate ), VehicleType ( vehicleType ) );

    if ( freeVehicles.size() == 0 ) {
        this->showDialog ( "No vehicles are free in given date range" );
        return;
    }

    gotoXY ( 0, 12 );
    cout << "|Registration no  |Seats |Price per Km  |" << endl;

    for ( auto & v : freeVehicles ) {

        stringstream price;
        price << fixed << setprecision ( 2 ) << v->getPricePerKm() << " Rs/Km";

        cout << "|" << left << setw ( 17 ) << v->getRegistrationNumber()
             << "|" << setw ( 6 ) << v->getSeats()
             << "|" << setw ( 14 ) << price.str()
             << "|" << endl;
    }

    int offset = int ( freeVehicles.size() ) + 2;

    gotoXY ( 0, 12 + offset );
    cout << registrationNoLabel;

    gotoXY ( int ( registrationNoLabel.length() ), 12 + offset );
    getline ( cin, registrationNo );

    /* Find vehicle with given registration number and create a trip */
    try {
        vehicle = this->db->getVehicle ( registrationNo );
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
        return;
    }

    /* The chosen vehicle must be one of the free vehicles that were listed */
    bool listed = false;
    for ( auto & v : freeVehicles ) {
        if ( v->getRecordId() == vehicle->getRecordId() ) listed = true;
    }

    if ( !listed ) {
        this->showDialog ( "That vehicle is not in the list of available vehicles" );
        return;
    }

    long userId = user->getRecordId();
    long vehicleId = vehicle->getRecordId();

    Trip * trip = NULL;

    try {
        trip = new Trip ( this->db->getVehicleRef()->getRecordForId ( vehicleId ), this->db->getUserRef()->getRecordForId ( userId ), Date ( startDate ), Date ( endDate ) );
        this->db->addNewRecord ( trip );

        stringstream ss;
        ss << "Trip Id : " << trip->getRecordId();
        showDialog ( "Trip added successfully", ss.str() );
    }
    catch ( const Error & e ) {
        showDialog ( e.getMessage() );
    }

    /* Release resources */
    delete trip;
}

void Application :: renderViewTripMenu ( ) const {

    string header = "Enter trip id :  ";

    long tripId = 0;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( int ( header.length() ), 1 );
    readValue ( tripId );

    /* Find Trip and display data */
    gotoXY ( 0, 3 );

    try {
        auto trip = this->db->getTripRef()->getRecordForId ( tripId );
        trip->display();

        cout << endl << "Press any key to continue";
        cin.get();
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
    }
}

void Application :: renderStartTripMenu ( ) const {

    string header = "Enter trip id :  ";
    string readingLabel = "Enter odometer reading :  ";

    long tripId = 0;
    long startReading = 0;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( 0, 2 );
    cout << readingLabel;

    gotoXY ( int ( header.length() ), 1 );
    readValue ( tripId );

    gotoXY ( int ( readingLabel.length() ), 2 );
    readValue ( startReading );

    Trip * modifiedTrip = NULL;

    try {
        auto trip = this->db->getTripRef()->getRecordForId ( tripId );

        if ( trip->isCompleted() ) {
            this->showDialog ( "This trip is already completed" );
            return;
        }

        if ( startReading <= 0 ) {
            this->showDialog ( "Odometer reading must be greater than 0" );
            return;
        }

        modifiedTrip = new Trip ( *trip );
        modifiedTrip->startTrip ( startReading );

        this->db->updateRecord ( modifiedTrip );
        showDialog ( "Trip started successfully" );
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
    }

    delete modifiedTrip;
}

void Application :: renderCompleteTripMenu ( ) const {

    string header = "Enter trip id :  ";
    string readingLabel = "Enter odometer reading :  ";

    long tripId = 0;
    long endReading = 0;

    this->clearScreen();

    gotoXY ( 0, 1 );
    cout << header;

    gotoXY ( 0, 2 );
    cout << readingLabel;

    gotoXY ( int ( header.length() ), 1 );
    readValue ( tripId );

    gotoXY ( int ( readingLabel.length() ), 2 );
    readValue ( endReading );

    Trip * modifiedTrip = NULL;

    try {
        auto trip = this->db->getTripRef()->getRecordForId ( tripId );

        if ( trip->getStartReading() == 0 ) {
            this->showDialog ( "This trip has not been started yet. Start it first (option 7)" );
            return;
        }

        if ( endReading < trip->getStartReading() ) {
            this->showDialog ( "End reading cannot be less than start reading" );
            return;
        }

        modifiedTrip = new Trip ( *trip );
        auto fare = modifiedTrip->completeTrip ( endReading );
        this->db->updateRecord ( modifiedTrip );

        stringstream ss;
        ss << "Total Fare : " << fare;
        showDialog ( "Trip completed successfully", ss.str() );
    }
    catch ( const Error & e ) {
        this->showDialog ( e.getMessage() );
    }

    delete modifiedTrip;
}

/* Draw a box made of '*' with a message inside, then wait for Enter */
void Application :: showDialog ( string message, string id ) const {

    string pressAnyKey = "Press any key to continue";

    int messageLength = int ( message.length() );
    int idLength = int ( id.length() );
    int infoLength = int ( pressAnyKey.length() );

    int leftOffset = 15;
    int lineOffset = 9;

    /* The box is 50 wide, but grows if the message is longer (the book's fixed width broke for long messages) */
    int length = 50;
    int widest = messageLength;
    if ( idLength > widest ) widest = idLength;
    if ( infoLength > widest ) widest = infoLength;
    if ( widest + 4 > length ) length = widest + 4;

    this->clearScreen();

    /* Line 1 */
    gotoXY ( leftOffset, lineOffset++ );
    cout << string ( length, '*' );

    /* Line 2 */
    gotoXY ( leftOffset, lineOffset );
    cout << "*";
    gotoXY ( leftOffset + length - 1, lineOffset++ );
    cout << "*";

    /* Line 3: the message */
    gotoXY ( leftOffset, lineOffset );
    cout << "*";
    gotoXY ( leftOffset + ( length - messageLength ) / 2, lineOffset );
    cout << message;
    gotoXY ( leftOffset + length - 1, lineOffset++ );
    cout << "*";

    /* Line 4: the id (only if given) */
    if ( id != "" ) {
        gotoXY ( leftOffset, lineOffset );
        cout << "*";
        gotoXY ( leftOffset + ( length - idLength ) / 2, lineOffset );
        cout << id;
        gotoXY ( leftOffset + length - 1, lineOffset++ );
        cout << "*";
    }

    /* Line 5: press any key */
    gotoXY ( leftOffset, lineOffset );
    cout << "*";
    gotoXY ( leftOffset + ( length - infoLength ) / 2, lineOffset );
    cout << pressAnyKey;
    gotoXY ( leftOffset + length - 1, lineOffset++ );
    cout << "*";

    /* Line 6 */
    gotoXY ( leftOffset, lineOffset );
    cout << "*";
    gotoXY ( leftOffset + length - 1, lineOffset++ );
    cout << "*";

    /* Line 7 */
    gotoXY ( leftOffset, lineOffset );
    cout << string ( length, '*' );

    /* Put the cursor below the box and wait until user presses Enter */
    gotoXY ( 0, lineOffset + 2 );
    cin.get();
}

void Application :: welcome ( ) {

    this->clearScreen();

    gotoXY ( 25, 9 );
    cout << "Welcome To Vehicle Rental System !";
    gotoXY ( 27, 12 );
    cout << "Press Enter to continue";

    /* Wait until user presses Enter */
    cin.get();

    this->renderMenu();
}

void Application :: start ( ) {
    welcome();
}

void Application :: cleanMemory ( ) {
    delete this->db;
    this->db = NULL;
}
