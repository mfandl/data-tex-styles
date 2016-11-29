//
//  ColorSetup.cpp
//  textil
//
//  Created by Matej Fandl on 14/10/15.
//
//

#include "ColorSetup.hpp"

string ColorSetup::toString(const string &name) {
    stringstream ss;
    ss << name << ";";
    ss << frontColorBorder << ";";
    ss << backColorBorder << ";";
    ss << bgColor << ";";
    ss << verticalColor[0] << ";";
    ss << verticalColor[1] << ";";
    ss << horizontalColor[0] << ";";
    ss << horizontalColor[1];
    
    return ss.str();
};

void ColorSetup::fromString(const string &str) {
    vector<string> tokens = splitString(str, ";");
    
    frontColorBorder = atof(tokens[1].c_str());
    backColorBorder = atof(tokens[2].c_str());
    
    stringstream ss;
    
    ss.str(tokens[3]);
    ss >> bgColor;
    
    ss.clear();
    ss.str(tokens[4]);
    ss >> verticalColor[0];
    
    ss.clear();
    ss.str(tokens[5]);
    ss >> verticalColor[1];
    
    ss.clear();
    ss.str(tokens[6]);
    ss >> horizontalColor[0];
    
    ss.clear();
    ss.str(tokens[7]);
    ss >> horizontalColor[1];
    
};