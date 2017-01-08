#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "mpUtils.hpp"
#include "ColorSetup.hpp"

const int pointCloudWidth = 640;
const int pointCloudHeight = 480;

class ofApp : public ofBaseApp {

public:
    ofEasyCam camera;
    
	void setup();
	void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    void guiColorEvent(ofxUIEventArgs &e);
    void guiPatternEvent(ofxUIEventArgs &e);
    void guiEvent4(ofxUIEventArgs &e);
    
private:
    
    ofVec3f vertices[pointCloudWidth][pointCloudHeight];
    ofColor colors[pointCloudWidth][pointCloudHeight];
    
    int pattern_dimensions_max[2] = {20, 20};
    int pattern_dimensions[2] = {pattern_dimensions_max[0], pattern_dimensions_max[1]};
    int **pattern;
    
    int patternCount = 0;
    vector<string> patterns;
    
    float lineWidth = 1;
    float lineWidthMax = 20;
    int step = 1;
    int stepMax = 20;
    int spreadMax = 100;
    int spread = 10;
    float maxFarBorder = 5000;
    float farBorder = maxFarBorder;
    float nearBorder = 1;
    
    float backColorBorder = 2000;
    float frontColorBorder = 1;
    
    bool useLines = true;
    bool useCameraColor = true;
    
    
    
    string imgResX = "12000";
    string imgResY = "9000";
    
    
    
    ofColor bgColor;
    
    // 0 - front, 1 - back
    ofColor horizontalColor[2] = {ofColor(0), ofColor(0)};
    ofColor verticalColor[2] = {ofColor(0), ofColor(0)};
    
    map<string, ColorSetup> colorsMap;
    vector<string> colorsNames;
    
    void drawPointCloud();
    
    void saveImage();
    
    // MARK: Helpers
    void buildPatternArray();
    
    ofColor getColorAt(int x, int y);
    ofVec3f getWorldCoordinateAt(int x, int y);
    
    void setColorSetup(const ColorSetup & setup);
    void patternFromString(string patternString);
    
    float getDistanceAt(int x, int y);
    void incrementZBy(int x, int y, float value);
    
    // MARK: Persistence
    
    void saveScene();
    void loadFromFile(const string &path);
    
    void saveColorSetup();
    void loadColorSetups();
    
    
    
    // MARK: Gui
    
    ofxUISuperCanvas *gui;
    ofxUISuperCanvas *gui2;
    ofxUISuperCanvas *gui3;
    ofxUISuperCanvas *gui4;
    
    void setupGui();
    
    void updateColorSliders();
    
    void savePattern();
    void loadPatterns();
    
    void updatePatternToggleMatrixSize();
    void addPatternToggleMatrix(int w, int h);
		
};
