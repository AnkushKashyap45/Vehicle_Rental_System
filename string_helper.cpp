/* Include custom headers */

#include "string_helper.h"

vector<string> split ( const string & s, char delimiter ) {

    vector<string> tokens;
    string token;
    istringstream tokenStream ( s );

    while ( getline ( tokenStream, token, delimiter ) ) {
        tokens.push_back ( token );
    }

    /* getline() silently drops an empty LAST field ("a;b;" -> only a, b). */
    /* Add it back so records with an empty last field (e.g. no email) keep the right number of fields. */
    if ( !s.empty() && s[s.length() - 1] == delimiter ) {
        tokens.push_back ( "" );
    }

    return tokens;
}
