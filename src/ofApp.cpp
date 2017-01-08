#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    loadPatterns();
    loadColorSetups();
    
    buildPatternArray();
    string path = "682";
    loadFromFile(path);
    
    ofEnableSmoothing();
    
    setupGui();
    updatePatternToggleMatrixSize();
}


//--------------------------------------------------------------
void ofApp::update(){}

//--------------------------------------------------------------
void ofApp::draw(){
    drawPointCloud();
}

void ofApp::drawPointCloud() {
    
    ofBackground(bgColor);
    camera.begin();
    
    int w = pointCloudWidth;
    int h = pointCloudHeight;
    ofMesh *rows = new ofMesh[h / step];
    ofMesh *cols = new ofMesh[w / step];
    
    ofPrimitiveMode mode = useLines ? OF_PRIMITIVE_TRIANGLES : OF_PRIMITIVE_POINTS;
    
    for (int i = 0; i < h / step; ++i) {
        rows[i].setMode(mode);
    }
    
    for (int i = 0; i < w / step; ++i) {
        cols[i].setMode(mode);
    }
    
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            float dist = getDistanceAt(x, y);
            if(dist > nearBorder && dist < farBorder && (x + step) <= pointCloudWidth) {
                
                ofVec3f worldCoord = getWorldCoordinateAt(x, y);
                
                
                ofVec3f nextWorldCoord;
                int lookupX = x;
                do {
                    lookupX += step;
                    nextWorldCoord = getWorldCoordinateAt(lookupX, y);
                } while (getDistanceAt(lookupX, y) == 0 && lookupX < pointCloudWidth);
                
                if (getDistanceAt(lookupX, y) < 5) continue;
                
                ofVec3f rowWorldCoord(worldCoord);
                ofVec3f worldScreenCoord = camera.worldToScreen(worldCoord);
                
                ofVec3f rowNextWorldCoord(nextWorldCoord);
                
                worldScreenCoord = camera.worldToScreen(rowNextWorldCoord);
                
                rowWorldCoord.z -= (spread * (-1 + 2 * pattern[(x / step) % pattern_dimensions[0]][(y / step) % pattern_dimensions[1]]));
                
                ofVec3f underRowWorldCoord(rowWorldCoord);
                underRowWorldCoord.y += lineWidth;
                
                rowNextWorldCoord.z -= (spread * (-1 + 2 * pattern[(lookupX / step) % pattern_dimensions[0]][(y / step) % pattern_dimensions[1]]));
                
                ofVec3f underRowNextWorldCoord(rowNextWorldCoord);
                underRowNextWorldCoord.y += lineWidth;
                
                ofColor rowColor;
                ofColor rowNextColor;
                int mappedDepth = ofMap(getDistanceAt(x, y), frontColorBorder, backColorBorder, 0, 255, true);
                int mappedNextDepth = ofMap(getDistanceAt(lookupX, y), frontColorBorder, backColorBorder, 0, 255, true);
                if (useCameraColor) {
                    rowColor = getColorAt(x, y);
                    rowNextColor = getColorAt(x + step, y);
                } else {
                    float sub = horizontalColor[0].r - horizontalColor[1].r;
                    rowColor.r = horizontalColor[0].r - (sub * ((float) mappedDepth / 255.0));
                    rowNextColor.r = horizontalColor[0].r - (sub * ((float) mappedNextDepth / 255.0));
                    
                    sub = horizontalColor[0].g - horizontalColor[1].g;
                    rowColor.g = horizontalColor[0].g - (sub * ((float) mappedDepth / 255.0));
                    rowNextColor.g = horizontalColor[0].g - (sub * ((float) mappedNextDepth / 255.0));
                    
                    sub = horizontalColor[0].b - horizontalColor[1].b;
                    rowColor.b = horizontalColor[0].b - (sub * ((float) mappedDepth / 255.0));
                    rowNextColor.b = horizontalColor[0].b - (sub * ((float) mappedNextDepth / 255.0));
                    
                }
                
                
                
                rows[y / step].addColor(rowColor);
                rows[y / step].addVertex(rowWorldCoord);
                
                rows[y / step].addColor(rowColor);
                rows[y / step].addVertex(underRowWorldCoord);
                
                rows[y / step].addColor(rowNextColor);
                rows[y / step].addVertex(underRowNextWorldCoord);
                
                rows[y / step].addColor(rowColor);
                rows[y / step].addVertex(rowWorldCoord);
                
                rows[y / step].addColor(rowNextColor);
                rows[y / step].addVertex(underRowNextWorldCoord);
                
                rows[y / step].addColor(rowNextColor);
                rows[y / step].addVertex(rowNextWorldCoord);
                
                ofVec3f nextColWorldCoord;
                int lookupY = y;
                
                do {
                    lookupY += step;
                    nextColWorldCoord = getWorldCoordinateAt(x, lookupY);
                } while (getDistanceAt(x, lookupY) == 0 && lookupY < pointCloudHeight);
                
                if (getDistanceAt(x, lookupY) < 5) continue;
                
                ofVec3f worldCoordRight = ofVec3f(worldCoord);
                worldCoord.x += lineWidth;
                
                worldScreenCoord = camera.worldToScreen(nextColWorldCoord);
                dist = ofDist(worldScreenCoord.x, worldScreenCoord.y, ofGetWidth() - mouseX, mouseY);
                
                ofVec3f nextColWorldCoordRight = ofVec3f(nextColWorldCoord);
                nextColWorldCoord.x += lineWidth;
                
                ofColor colColor;
                ofColor colNextColor;
                
                int mappedNextColDepth = ofMap(getDistanceAt(x, lookupY), 500, 2000, 0, 255, true);

                if (useCameraColor) {
                    colColor.r = 255 - rowColor.r;
                    colColor.g = 255 - rowColor.g;
                    colColor.b = 255 - rowColor.b;
                    
                    colNextColor.r = 255 - rowColor.r;
                    colNextColor.g = 255 - rowColor.g;
                    colNextColor.b = 255 - rowColor.b;
                } else {
                    float sub = verticalColor[0].r - verticalColor[1].r;
                    colColor.r = verticalColor[0].r - (sub * ((float) mappedDepth / 255.0));
                    colNextColor.r = verticalColor[0].r - (sub * ((float) mappedNextColDepth / 255.0));
                    
                    sub = verticalColor[0].g - verticalColor[1].g;
                    colColor.g = verticalColor[0].g - (sub * ((float) mappedDepth / 255.0));
                    colNextColor.g = verticalColor[0].g - (sub * ((float) mappedNextDepth / 255.0));
                    
                    sub = verticalColor[0].b - verticalColor[1].b;
                    colColor.b = verticalColor[0].b - (sub * ((float) mappedDepth / 255.0));
                    colNextColor.b = verticalColor[0].b - (sub * ((float) mappedNextColDepth / 255.0));
                }
                
                int colIndex = x / step;
                
                cols[colIndex].addColor(colColor);
                cols[colIndex].addVertex(worldCoord);
                
                cols[colIndex].addColor(colNextColor);
                cols[colIndex].addVertex(nextColWorldCoord);
                
                cols[colIndex].addColor(colNextColor);
                cols[colIndex].addVertex(nextColWorldCoordRight);
                
                cols[colIndex].addColor(colColor);
                cols[colIndex].addVertex(worldCoord);
                
                cols[colIndex].addColor(colNextColor);
                cols[colIndex].addVertex(nextColWorldCoordRight);
                
                cols[colIndex].addColor(colColor);
                cols[colIndex].addVertex(worldCoordRight);
            }
        }
    }
    
    glPointSize(lineWidth);
    glLineWidth(lineWidth);
    ofPushMatrix();
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000);
    ofEnableDepthTest();
    
    int numCols = w / step;
    for (int i = 0; i < numCols; ++i) {
        cols[i].drawFaces();
    }
    
    int numRows = h / step;
    for (int i = 0; i < numRows; ++i) {
        rows[i].drawFaces();
    }
    ofDisableDepthTest();
    ofPopMatrix();
    
    delete [] rows;
    delete [] cols;
    
    camera.end();
}

void ofApp::saveImage() {
    ofFbo fbo;
    fbo.allocate(atoi(imgResX.c_str()), atoi(imgResY.c_str()));
    fbo.begin();
    drawPointCloud();
    
    
    fbo.end();
    
    
    ofImage img;
    img.allocate(fbo.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR);
    fbo.readToPixels(img);
    
    img.save(ofGetTimestampString() + ".png");
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        saveScene();
    } else if (key == 'h') {
        gui->toggleVisible();
        gui2->toggleVisible();
        gui3->toggleVisible();
        gui4->toggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit() {
    delete gui;
}


// MARK: Helpers

void ofApp::setColorSetup(const ColorSetup &setup) {
    frontColorBorder = setup.frontColorBorder;
    backColorBorder = setup.backColorBorder;
    bgColor = setup.bgColor;
    verticalColor[0] = setup.verticalColor[0];
    verticalColor[1] = setup.verticalColor[1];
    horizontalColor[0] = setup.horizontalColor[0];
    horizontalColor[1] = setup.horizontalColor[1];
}

void ofApp::patternFromString(string patternString) {
    vector<string> tokens = splitString(patternString, " ");
    
    pattern_dimensions[0] = atoi(tokens[0].c_str());
    pattern_dimensions[1] = atoi(tokens[1].c_str());
    
    int index = 2;
    for (int i = 0; i < pattern_dimensions_max[1]; ++i) {
        for (int j = 0; j < pattern_dimensions_max[0]; ++j) {
            pattern[i][j] = atoi(tokens[index++].c_str());
        }
    }
}

ofColor ofApp::getColorAt(int x, int y) {
    return colors[x][y];
}

ofVec3f ofApp::getWorldCoordinateAt(int x, int y) {
    return vertices[x][y];
}

float ofApp::getDistanceAt(int x, int y) {
    return vertices[x][y].z;
}

void ofApp::incrementZBy(int x, int y, float value) {
    vertices[x][y].z += value;
}

// MARK: Persistence - Saving

void ofApp::saveScene() {
    ofFile file;
    file.open(ofToDataPath("colorscene" + ofGetTimestampString()), ofFile::WriteOnly, false);
    
    file << pattern_dimensions_max[0] << " " << pattern_dimensions_max[1] << endl;
    file << pattern_dimensions[0] << " " << pattern_dimensions[1] << endl;
    for (int i = 0; i < pattern_dimensions_max[1]; ++i) {
        for (int j = 0; j < pattern_dimensions_max[0]; ++j) {
            file << pattern[i][j] << " ";
        }
    }
    file << endl;
    
    file << lineWidth << endl;
    file << step << endl;
    file << spread << endl;
    file << farBorder << endl;
    file << nearBorder << endl;
    file << useLines << endl;
    file << useCameraColor << endl;
    file << bgColor << endl;
    file << horizontalColor[0] << endl;
    file << horizontalColor[1] << endl;
    file << verticalColor[0] << endl;
    file << verticalColor[1] << endl;
    
    file << frontColorBorder << endl;
    file << backColorBorder << endl;
    
    file << endl;
    
    for(int y = 0; y < pointCloudHeight; y += 1) {
        for(int x = 0; x < pointCloudWidth; x += 1) {
            file << getWorldCoordinateAt(x, y) << ";" << getColorAt(x, y) << endl;
        }
    }
    
    file.close();
}

void ofApp::savePattern() {
    ofFile file;
    file.open(ofToDataPath("patterns"), ofFile::Append, false);
    file << pattern_dimensions[0] << " " << pattern_dimensions[1] << " ";
    
    for (int i = 0; i < pattern_dimensions_max[1]; ++i) {
        for (int j = 0; j < pattern_dimensions_max[0]; ++j) {
            file << pattern[i][j] << " ";
        }
    }
    file << endl;
    file.close();
}

void ofApp::buildPatternArray() {
    pattern = new int*[pattern_dimensions_max[1]];
    for (int i = 0; i < pattern_dimensions_max[0]; ++i) {
        pattern[i] = new int[pattern_dimensions_max[1]];
        for (int j = 0; j < pattern_dimensions_max[1]; ++j) {
            pattern[i][j] = 0;
        }
    }
}

void ofApp::saveColorSetup() {
    ofFile file;
    file.open(ofToDataPath("colors"), ofFile::Append, false);
    ColorSetup colorsToSave;
    colorsToSave.frontColorBorder = frontColorBorder;
    colorsToSave.backColorBorder = backColorBorder;
    colorsToSave.bgColor = bgColor;
    colorsToSave.verticalColor[0] = verticalColor[0];
    colorsToSave.verticalColor[1] = verticalColor[1];
    colorsToSave.horizontalColor[0] = horizontalColor[0];
    colorsToSave.horizontalColor[1] = horizontalColor[1];
    file << colorsToSave.toString(ofGetTimestampString()) << endl;
    file.close();
}



// MARK: Persistence - Loading

void ofApp::loadFromFile(const string &path) {
    ofFile file;
    
    if (!file.open(path, ofFile::ReadOnly, false)) {
        cerr << "failed to open file" << endl;
    }
    ofBuffer buff = file.readToBuffer();
    
    ofBuffer::Lines lines = buff.getLines();
    
    ofBuffer::Line l = lines.begin();
    
    
    string currentLine;
    
    vector<string> splitLine = splitString(l.asString(), " ");
    pattern_dimensions_max[0] = atoi(splitLine[0].c_str());
    pattern_dimensions_max[1] = atoi(splitLine[1].c_str());
    
    splitLine = splitString((++l).asString(), " ");
    pattern_dimensions[0] = atoi(splitLine[0].c_str());
    pattern_dimensions[1] = atoi(splitLine[1].c_str());
    
    splitLine = splitString((++l).asString(), " ");
    int index = 0;
    for (int i = 0; i < pattern_dimensions_max[1]; ++i) {
        for (int j = 0; j < pattern_dimensions_max[0]; ++j) {
            pattern[i][j] = atoi(splitLine[index++].c_str());
        }
    }
    
    lineWidth = atof((++l).asString().c_str());
    step = atoi((++l).asString().c_str());
    spread = atoi((++l).asString().c_str());
    farBorder = atof((++l).asString().c_str());
    nearBorder = atof((++l).asString().c_str());
    useLines = (++l).asString().compare("1") == 0 ? true : false;
    
    useCameraColor = (++l).asString().compare("1") == 0 ? true : false;
    
    splitLine = splitString((++l).asString(), ", ");
    bgColor = ofColor(atoi(splitLine[0].c_str()), atoi(splitLine[1].c_str()), atoi(splitLine[2].c_str()));
    
    splitLine = splitString((++l).asString(), ", ");
    horizontalColor[0] = ofColor(atoi(splitLine[0].c_str()), atoi(splitLine[1].c_str()), atoi(splitLine[2].c_str()));
    
    splitLine = splitString((++l).asString(), ", ");
    horizontalColor[1] = ofColor(atoi(splitLine[0].c_str()), atoi(splitLine[1].c_str()), atoi(splitLine[2].c_str()));
    
    splitLine = splitString((++l).asString(), ", ");
    verticalColor[0] = ofColor(atoi(splitLine[0].c_str()), atoi(splitLine[1].c_str()), atoi(splitLine[2].c_str()));
    
    splitLine = splitString((++l).asString(), ", ");
    verticalColor[1] = ofColor(atoi(splitLine[0].c_str()), atoi(splitLine[1].c_str()), atoi(splitLine[2].c_str()));
    
    frontColorBorder = atof((++l).asString().c_str());
    backColorBorder = atof((++l).asString().c_str());
    
    while (true) {
        currentLine = (++l).asString();
        if (currentLine.compare("") == 0) break;
    }
    
    
    int i = 0;
    while (true) {
        currentLine = (++l).asString();
        if (currentLine.compare("") == 0) {
            break;
        }
        string positionString = currentLine.substr(0, currentLine.find(";"));
        vector<string> splitStr = splitString(positionString, ", ");
        ofVec3f vertex;
        vertex.x = atof(splitStr[0].c_str());
        vertex.y = atof(splitStr[1].c_str());
        vertex.z = atof(splitStr[2].c_str());
        
        string colorString = currentLine.substr(currentLine.find(";") + 1, currentLine.length());
        splitStr = splitString(colorString, ", ");
        
        ofColor color;
        color.r = atoi(splitStr[0].c_str());
        color.g = atoi(splitStr[1].c_str());
        color.b = atoi(splitStr[2].c_str());
        color.a = atoi(splitStr[3].c_str());
        
        int x = i == 0 ? 0 : (i % 640);
        int y = i / 640;
        
        if (x < pointCloudWidth) {
            vertices[x][y] = vertex;
            colors[x][y] = color;
        }
        
        if (currentLine == lines.end().asString()) {
            break;
        }
        
        ++i;
    }
    
    file.close();
    
}

void ofApp::loadPatterns() {
    ofBuffer buff = ofBufferFromFile(ofToDataPath("patterns"));
    
    ofBuffer::Line l = buff.getLines().begin();
    string line;
    while ((line = (++l).asString()) != "") {
        patterns.push_back(line);
    }
}

void ofApp::loadColorSetups() {
    ofBuffer buff = ofBufferFromFile(ofToDataPath("colors"));
    ofBuffer::Line l = buff.getLines().begin();
    string line;
    while ((line = (++l).asString()) != "") {
        string name = splitString(line, ";")[0];
        colorsNames.push_back(name);
        colorsMap[name] = line;
    }
}

// MARK: Gui

void ofApp::setupGui() {
    gui = new ofxUISuperCanvas("1");
    gui->addIntSlider("SPREAD", 1, spreadMax, &spread);
    gui->addIntSlider("STEP", 1, stepMax, &step);
    gui->addSlider("LINE_WIDTH", 0.01, lineWidthMax, &lineWidth);
    gui->addIntSlider("PATTERN_HORIZONTAL", 1, pattern_dimensions_max[0], &pattern_dimensions[0]);
    gui->addIntSlider("PATTERN_VERTICAL", 1, pattern_dimensions_max[1], &pattern_dimensions[1]);
    addPatternToggleMatrix(pattern_dimensions_max[0], pattern_dimensions_max[1]);
    
    gui->addSlider("CLOSER_THAN", 1, maxFarBorder, &farBorder);
    gui->addSlider("FURTHER_THAN", 1, maxFarBorder, &nearBorder);
    
    gui->addLabelButton("CLEAR", false, false);
    gui->addToggle("LINES", &useLines);
    gui->addToggle("CAMERA_COLOR", &useCameraColor);
    
    gui->addTextInput("IMAGE_RES_X", imgResX);
    gui->addTextInput("IMAGE_RES_Y", imgResY);
    gui->addLabelButton("SAVE_IMAGE", false, false);
    
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
    gui2 = new ofxUISuperCanvas("2");
    gui2->setPosition(212, 0);
    
    gui2->addIntSlider("BACKGROUND_R", 0, 255, bgColor.r);
    gui2->addIntSlider("BACKGROUND_G", 0, 255, bgColor.g);
    gui2->addIntSlider("BACKGROUND_B", 0, 255, bgColor.b);
    
    gui2->addSpacer();
    gui2->addSpacer();
    
    gui2->addIntSlider("HORIZONTAL_FRONT_R", 0, 255, horizontalColor[0].r);
    gui2->addIntSlider("HORIZONTAL_FRONT_G", 0, 255, horizontalColor[0].g);
    gui2->addIntSlider("HORIZONTAL_FRONT_B", 0, 255, horizontalColor[0].b);
    
    gui2->addSpacer();
    gui2->addSpacer();
    
    gui2->addIntSlider("HORIZONTAL_REAR_R", 0, 255, horizontalColor[1].r);
    gui2->addIntSlider("HORIZONTAL_REAR_G", 0, 255, horizontalColor[1].g);
    gui2->addIntSlider("HORIZONTAL_REAR_B", 0, 255, horizontalColor[1].b);
    
    gui2->addSpacer();
    gui2->addSpacer();
    
    gui2->addIntSlider("VERTICAL_FRONT_R", 0, 255, verticalColor[0].r);
    gui2->addIntSlider("VERTICAL_FRONT_G", 0, 255, verticalColor[0].g);
    gui2->addIntSlider("VERTICAL_FRONT_B", 0, 255, verticalColor[0].b);
    
    gui2->addSpacer();
    gui2->addSpacer();
    
    gui2->addIntSlider("VERTICAL_REAR_R", 0, 255, verticalColor[1].r);
    gui2->addIntSlider("VERTICAL_REAR_G", 0, 255, verticalColor[1].g);
    gui2->addIntSlider("VERTICAL_REAR_B", 0, 255, verticalColor[1].b);
    
    gui2->addSpacer();
    gui2->addSpacer();
    
    gui2->addSlider("FRONT_CLR_BORDER", 1, maxFarBorder, &frontColorBorder);
    gui2->addSlider("BACK_CLR_BORDER", 1, maxFarBorder, &backColorBorder);
    
    
    gui2->autoSizeToFitWidgets();
    ofAddListener(gui2->newGUIEvent, this, &ofApp::guiColorEvent);
    
    gui3 = new ofxUISuperCanvas("3");
    gui3->setPosition(212 * 2, 0);
    gui3->addLabelButton("SAVE_PATTERN", false, false);
    gui3->addSpacer();
    gui3->addDropDownList("PATTERNS", patterns);
    
    gui3->autoSizeToFitWidgets();
    ofAddListener(gui3->newGUIEvent, this, &ofApp::guiPatternEvent);
    
    gui4 = new ofxUISuperCanvas("3");
    gui4->setPosition(212 * 3, 0);
    
    gui4->addLabelButton("SAVE_COLORS", false, false);
    gui4->addSpacer();
    gui4->addDropDownList("COLORS", colorsNames);
    gui4->autoSizeToFitWidgets();
    ofAddListener(gui4->newGUIEvent, this, &ofApp::guiEvent4);
}

void ofApp::guiEvent(ofxUIEventArgs &e) {
    if(e.getName() == "PATTERN_HORIZONTAL" || e.getName() == "PATTERN_VERTICAL") {
        updatePatternToggleMatrixSize();
        
    } else if (ofIsStringInString(e.getName(), "PATTERN_MATRIX")) {
        ofxUIToggleMatrix *matrix = (ofxUIToggleMatrix *) e.widget->getParent();
        for(int y = 0; y < pattern_dimensions[1]; ++y) {
            for (int x = 0; x < pattern_dimensions[0]; ++x) {
                ofxUIToggle *toggle = matrix->getToggle(y, x);
                pattern[x][y] = toggle->getValue() ? 1 : 0;
            }
        }
    } else if (e.getName() == "CLEAR") {
        ofxUIToggleMatrix *matrix = (ofxUIToggleMatrix *) gui->getWidget("PATTERN_MATRIX");
        for (int i = 0; i < pattern_dimensions_max[0]; ++i) {
            for (int j = 0; j < pattern_dimensions_max[1]; ++j) {
                ofxUIToggle *toggle = matrix->getToggle(j, i);
                toggle->setValue(false);
                pattern[i][j] = 0;
            }
        }
    } else if (e.getName() == "SAVE_IMAGE") {
        ofxUILabelButton *b = (ofxUILabelButton *) gui->getWidget("SAVE_IMAGE");
        if (b->getValue()) {
            saveImage();
        }
    } else if (e.getName() == "IMAGE_RES_X") {
        ofxUITextInput *textInput = (ofxUITextInput *) gui->getWidget("IMAGE_RES_X");
        imgResX = textInput->getTextString();
        
    } else if (e.getName() == "IMAGE_RES_Y") {
        ofxUITextInput *textInput = (ofxUITextInput *) gui->getWidget("IMAGE_RES_Y");
        imgResY = textInput->getTextString();
    }
    
}

void ofApp::guiColorEvent(ofxUIEventArgs &e) {
    ofxUIIntSlider *slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_R");
    bgColor.r = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_G");
    bgColor.g = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_B");
    bgColor.b = slider->getValue();
    
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_R");
    horizontalColor[0].r = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_G");
    horizontalColor[0].g = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_B");
    horizontalColor[0].b = slider->getValue();
    
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_R");
    horizontalColor[1].r = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_G");
    horizontalColor[1].g = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_B");
    horizontalColor[1].b = slider->getValue();
    
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_R");
    verticalColor[0].r = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_G");
    verticalColor[0].g = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_B");
    verticalColor[0].b = slider->getValue();
    
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_R");
    verticalColor[1].r = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_G");
    verticalColor[1].g = slider->getValue();
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_B");
    verticalColor[1].b = slider->getValue();
}

void ofApp::guiPatternEvent(ofxUIEventArgs &e) {
    if (e.getName() == "SAVE_PATTERN") {
        ofxUILabelButton *b = (ofxUILabelButton *) gui3->getWidget(e.getName());
        if (b->getValue()) {
            savePattern();
            loadPatterns();
            ofxUIDropDownList *list = (ofxUIDropDownList *) gui3->getWidget("PATTERNS");
            list->addToggle(patterns[patterns.size() - 1]);
        }
    } else if (e.getName() == "PATTERNS") {
        ofxUIDropDownList *list = (ofxUIDropDownList *) gui3->getWidget(e.getName());
        vector<string> selectedNames = list->getSelectedNames();
        if (selectedNames.size() > 0) {
            string pattern = selectedNames[0];
            patternFromString(pattern);
            updatePatternToggleMatrixSize();
        }
    }
}

void ofApp::guiEvent4(ofxUIEventArgs &e) {
    if (e.getName() == "SAVE_COLORS") {
        ofxUILabelButton *b = (ofxUILabelButton *) gui4->getWidget(e.getName());
        if (b->getValue()) {
            saveColorSetup();
            loadColorSetups();
            ofxUIDropDownList *list = (ofxUIDropDownList *) gui4->getWidget("COLORS");
            list->addToggle(colorsNames[colorsNames.size() - 1]);
        }
    } else if (e.getName() == "COLORS") {
        ofxUIDropDownList *list = (ofxUIDropDownList *) gui4->getWidget(e.getName());
        vector<string> selectedNames = list->getSelectedNames();
        if (selectedNames.size() > 0) {
            string name = selectedNames[0];
            setColorSetup(colorsMap[name]);
            updateColorSliders();
        }
    }
}

void ofApp::updateColorSliders() {
    ofxUIIntSlider *slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_R");
    slider->setValue(bgColor.r);
    slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_G");
    slider->setValue(bgColor.g);
    slider = (ofxUIIntSlider *) gui2->getWidget("BACKGROUND_B");
    slider->setValue(bgColor.b);
    
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_R");
    slider->setValue(horizontalColor[0].r);
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_G");
    slider->setValue(horizontalColor[0].g);
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_FRONT_B");
    slider->setValue(horizontalColor[0].b);
    
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_R");
    slider->setValue(horizontalColor[1].r);
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_G");
    slider->setValue(horizontalColor[1].g);
    slider = (ofxUIIntSlider *) gui2->getWidget("HORIZONTAL_REAR_B");
    slider->setValue(horizontalColor[1].b);
    
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_R");
    slider->setValue(verticalColor[0].r);
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_G");
    slider->setValue(verticalColor[0].g);
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_FRONT_B");
    slider->setValue(verticalColor[0].b);
    
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_R");
    slider->setValue(verticalColor[1].r);
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_G");
    slider->setValue(verticalColor[1].g);
    slider = (ofxUIIntSlider *) gui2->getWidget("VERTICAL_REAR_B");
    slider->setValue(verticalColor[1].b);
}

void ofApp::addPatternToggleMatrix(int w, int h) {
    gui->addToggleMatrix("PATTERN_MATRIX", h, w);
}

void ofApp::updatePatternToggleMatrixSize() {
    ofxUIToggleMatrix *matrix = (ofxUIToggleMatrix *) gui->getWidget("PATTERN_MATRIX");
    for(int y = 0; y < pattern_dimensions_max[1]; ++y) {
        for (int x = 0; x < pattern_dimensions_max[0]; ++x) {
            ofxUIToggle *toggle = matrix->getToggle(y, x);
            if (y < pattern_dimensions[1] && x < pattern_dimensions[0]) {
                toggle->setVisible(true);
            } else {
                toggle->setVisible(false);
            }
        }
    }
    
    for(int y = 0; y < pattern_dimensions_max[1]; ++y) {
        for (int x = 0; x < pattern_dimensions_max[0]; ++x) {
            ofxUIToggle *toggle = matrix->getToggle(y, x);
            
            pattern[x][y] == 1 ? toggle->setValue(true) : toggle -> setValue(false);
        }
    }
}
