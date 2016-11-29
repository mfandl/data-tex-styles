//
//  ColorSetup.hpp
//  textil
//
//  Created by Matej Fandl on 14/10/15.
//
//

#ifndef ColorSetup_hpp
#define ColorSetup_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include "ofMain.h"
#include "mpUtils.hpp"


using namespace std;

class ColorSetup {
public:
    float frontColorBorder;
    float backColorBorder;
    
    ofColor bgColor;
    ofColor horizontalColor[2] = {ofColor(0), ofColor(0)};
    ofColor verticalColor[2] = {ofColor(0), ofColor(0)};
    
    
    
    ColorSetup() {
        frontColorBorder = 0;
        backColorBorder = 0;
    }
    
    ColorSetup(const string & csv) {
        frontColorBorder = 0;
        backColorBorder = 0;
        fromString(csv);
    }
    
    //TODO: operator
    void fromString(const string & str);
    string toString(const string & name);
};

#endif /* ColorSetup_hpp */
