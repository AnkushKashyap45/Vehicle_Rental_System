#ifndef users_h
#define users_h

/* Include custom headers */

#include "storable.cpp"

/* Include standard cpp headers */

#include <string>

using namespace std;

/* User class which helps in managing user-related data */

class User : public Storable {

private:

    /* Name of user */
    string name;

    /* Contact number of user */
    string contact;

    /* Email-id of user */
    string email;

public:

    /*
        Constructor
        input : Name of user.
        input : Contact number of user.
        input : Email Id of user.
        input : recordId (defaults to 0)
    */

    User ( string name, string contact, string email, long recordId = 0 );

    /*
        Getter methods with const attribute i.e. this method is not changing any data of the current object
        If this method is not marked as constant, then it cannot be called from constant object of this class.
    */

    string getName ( ) const;

    string getContact ( ) const;

    string getEmail ( ) const;

    /*
        Set name of user
        input: New name of user
    */

    void setName ( string newName );

    /*
        Set contact number of user
        input: New contact number of user
    */

    void setContact ( string newContact );

    /*
        Set email-id of user
        input: New email-id of user
    */

    void setEmail ( string newEmail );

    /* Display record on screen */

    void display ( ) const;

    /* Overridden virtual method */

    string toString ( ) const;

    void setDataFrom ( Storable * s );
};

#endif
