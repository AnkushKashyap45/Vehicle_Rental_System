#ifndef Date_h
#define Date_h

/* Include custom headers */

#include "storable.cpp"

/* Include standard cpp headers */

#include <ctime>
#include <string>

using namespace std;

typedef struct Date : Serializable {

private:

    /* Structure holding date & time info */
    struct tm date;

    bool empty;

public:

    /* Constructor, create date from string (format d/m/yyyy) */
    Date ( string dateString );

    /* Create date from current time */
    Date ( );

    /* Add given number of days in this date */
    Date operator + ( int days ) const;

    /* If this date is greater than given date */
    bool operator > ( Date other ) const;

    /* If this date is greater than or equal to given date */
    bool operator >= ( Date other ) const;

    /* If this date is less than given date */
    bool operator < ( Date other ) const;

    /* If this date is less than or equal to given date */
    bool operator <= ( Date other ) const;

    bool isEmpty ( ) const;

    string toString ( ) const;

} Date;

#endif
