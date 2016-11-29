//
//  mpUtils.cpp
//  textil
//
//  Created by Matej Fandl on 14/10/15.
//
//

#include "mpUtils.hpp"

vector<string> splitString(const string text, const string delimiter) {
    vector<string> tokens;
    string workingText = text;
    int delimiterPosition = 0;
    while ((delimiterPosition = workingText.find(delimiter)) >= 0) {
        tokens.push_back(workingText.substr(0, delimiterPosition));
        workingText = workingText.substr(delimiterPosition + delimiter.length(), workingText.length());
    }
    if (workingText.length() > 0) tokens.push_back(workingText);
    
    return tokens;
}

