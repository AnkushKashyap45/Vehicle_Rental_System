#ifndef serializable_cpp
#define serializable_cpp

/* Include standard cpp headers */

#include <string>

using namespace std;

/*
    This is Serializable class and it is an abstract class.
    Any class derived from this class must implement 'toString' method
*/

class Serializable {

public:

    /* Pure virtual function */
    /* 'Serializable' does not know what 'toString' method will do */
    /* It is the responsibility of derived class to implement this method */
    /* Each derived class has its own 'toString' implementation. */

    virtual string toString() const = 0;

    /* Virtual destructor so objects are cleaned up correctly via base pointers */
    virtual ~Serializable() {}
};

#endif
