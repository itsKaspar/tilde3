
// ###### Default Scene
// ###### Startup Scene
// ###### Kaspar Ravel - 2020

#pragma once

#ifndef SUNSET
#define SUNSET

//#include "ofxOpenCv.h"
#include "ofMain.h" 
#include "ofxGui.h"
#include "Scene.h"


class Sunset : public Scene
{
public:

	// ##### Basic Functions

	void setup();
	void update();
	void draw();
	void reset();

	// ##### GUI Setup

	ofParameterGroup gui();

	// ##### Layer Settings

	void setColor1(ofColor color1) { c1 = color1; }
	void setColor2(ofColor color2) { c2 = color2; }
	void setOpacity(int o) { opacity = o; }

	ofColor c1;
	ofColor c2;
	int opacity;

	// ##### Webcam

	//bool toggleWebcam = false;
	ofVideoGrabber webcam;
	ofVideoPlayer video;

	//ofxCvColorImage color;
	//ofxCvGrayscaleImage gray;

private:
};

#endif