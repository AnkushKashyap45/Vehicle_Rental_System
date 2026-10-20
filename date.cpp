/* Include custom headers */

#include "date.h"
#include "string_helper.h"

/* Include standard cpp headers */

#include <sstream>
#include <vector>
#include <cstdlib>

const char DELIMITER = '/' ;

Date :: Date ( ) {

    /* create date from current time */

    time_t now = time(NULL);

    /* 'localtime' returns pointer to statically allocated memory, and we want value pointed by it. */
    this->empty = false;
    this->date = *localtime(&now);
}

Date :: Date ( string dateString /* Must be in d/m/yyyy */ ) {

    /* Start with a clean (all zero) structure so an empty date never holds garbage */
    this->date = tm();

    if ( dateString.length() < 8 ) {
        this->empty = true;
        return;
    }

    /* Split string with given delimiter */
    vector<string> dateComponents = split ( dateString, DELIMITER );

    if ( dateComponents.size() < 3 ) {
        this->empty = true;
        return;
    }

    /* Separate out components (stoi throws if text is not a number) */
    int day, month, year;

    try {
        day = stoi ( dateComponents[0] );
        month = stoi ( dateComponents[1] );
        year = stoi ( dateComponents[2] );
    }
    catch ( ... ) {
        this->empty = true;
        return;
    }

    this->empty = false;

    time_t now = time(NULL);
    this->date = *localtime(&now);

    this->date.tm_year = year - 1900;
    this->date.tm_mon = month - 1;
    this->date.tm_mday = day;
    this->date.tm_hour = 0;
    this->date.tm_min = 0;
    this->date.tm_sec = 0;
    this->date.tm_isdst = -1;

    /* mktime fixes overflowing values (e.g. 32 Jan becomes 1 Feb) */
    mktime ( &this->date );
}

bool Date :: isEmpty ( ) const {
    return this->empty;
}

string Date :: toString ( ) const {

    if ( this->empty ) {
        return "";
    }

    stringstream ss;
    ss << this->date.tm_mday << "/" << this->date.tm_mon + 1 << "/" << this->date.tm_year + 1900;

    return ss.str();
}

Date Date :: operator + ( int days ) const {

    /* Create a new date */
    Date newDate = *this;

    if ( newDate.isEmpty() ) {
        return newDate;
    }

    /* Modify days */
    newDate.date.tm_mday += days;

    /* Create modified date */
    time_t adjustedDate = mktime ( &newDate.date );

    /* Convert time to struct */
    newDate.date = *localtime ( &adjustedDate );

    return newDate;
}

bool Date :: operator > ( Date other ) const {

    if ( this->isEmpty() || other.isEmpty() ) {
        return false;
    }

    int yearDiff = this->date.tm_year - other.date.tm_year;
    int monthDiff = this->date.tm_mon - other.date.tm_mon;
    int dayDiff = this->date.tm_mday - other.date.tm_mday;

    if ( yearDiff != 0 ) {
        return yearDiff > 0;
    } else if ( monthDiff != 0 ) {
        return monthDiff > 0;
    } else {
        return dayDiff > 0;
    }
}

bool Date :: operator >= ( Date other ) const {
    return !( *this < other );
}

bool Date :: operator < ( Date other ) const {

    if ( this->isEmpty() || other.isEmpty() ) {
        return false;
    }

    int yearDiff = this->date.tm_year - other.date.tm_year;
    int monthDiff = this->date.tm_mon - other.date.tm_mon;
    int dayDiff = this->date.tm_mday - other.date.tm_mday;

    if ( yearDiff != 0 ) {
        return yearDiff < 0;
    } else if ( monthDiff != 0 ) {
        return monthDiff < 0;
    } else {
        return dayDiff < 0;
    }
}

bool Date :: operator <= ( Date other ) const {
    return !( *this > other );
}
