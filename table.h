#ifndef table_h
#define table_h

/* Include custom headers */

#include "storable.cpp"
#include "error.cpp"

/* Include standard cpp headers */

#include <vector>
#include <string>
#include <fstream>

using namespace std;

/*
    This is templated (generic) class. It represents in-memory database table.
    Table can be of type 'Vehicle', 'User', 'Trip'.
*/

template <class T> class Table {

private:

    /* Name of database file */
    string fileName;

    /* File stream used to read and write data from database file */
    fstream fileStream;

    /* Original data is stored here */
    /* 'vector' is a container type and behaves like dynamic array */
    vector<Storable *> * records = NULL;

    /* This is a private method */
    /* We will share pointer of record with recordId = 'recordId' */
    /* This is intentional, hence it is kept as private method */

    T * getReferenceOfRecordForId ( long recordId ) const;

    /* This is a private function. It is called after inserting new record or updating data */
    /* This function writes data to a file */

    void writeTofile ( );

public:

    /*
        Constructor
        input : fileName of database.
        Can throw 'MemoryError'
    */

    Table ( string fileName );
    ~Table ( );

    /*
        Get 'recordId' for next record
    */

    long getNextRecordId ( ) const;

    /*
        Add new record to database
        This method can throw 'MemoryError' or 'IOError'
    */

    const T * const addNewRecord ( T data );

    /*
        Update record in database
        input: new Record of generic type 'T'.
        This method can throw 'IOError' or 'NoSuchRecordError'
    */

    void updateRecord ( T updatedRecord );

    /*
        Get record for particular 'recordId'
        input: recordId of record

        We will return const T * const (constant pointer to constant record)
        So user cannot modify that record accidentally.

        This method can throw 'NoSuchRecord' Exception
    */

    const T * const getRecordForId ( long recordId ) const;

    /* Class database can use this class' private methods and variables */
    /* We will use 'getReferenceRecordForId' to get actual record reference */

    friend class Database;
};

#endif
