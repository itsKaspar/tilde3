
// ###### Default Scene
// ###### Startup Scene
// ###### Kaspar Ravel - 2020

#include "Sunset.h"

void Sunset::reset() {

}

ofParameterGroup Sunset::gui() {

	// ##### GUI Setup

	params.setName("Sunset");
	//params.add(scale.set("Scale", 0.1, 0.01, 1)); // make this a type variable
	//params.add(rotation.set("Rotation", ofVec3f(0, 1, 0.5), ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1)));
	return params;
}

void Sunset::setup() {
	int w = ofGetWidth();
	int h = ofGetHeight();
	webcam.setup(w, h);
	//video.load("lyt01.avi");
	//video.play();

	//color.allocate(webcam.getWidth(), webcam.getHeight());
	//gray.allocate(webcam.getWidth(), webcam.getHeight());
}

void Sunset::update() {
	webcam.update();
	//video.update();
	//if (webcam.isFrameNew())
	//{
		//color.setFromPixels(webcam.getPixels());
		//gray = color;
	//}

}

void Sunset::draw() {

	//color.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);
	//gray.draw(ofGetWidth() / 2, ofGetHeight() / 2);

	// Webcam 
	//webcam.draw(-ofGetWidth()/2,-ofGetHeight()/2, ofGetWidth(), ofGetHeight());               // PUT THIS INTO ITS OWN SCENE
	//video.draw(-ofGetWidth() / 2, -ofGetHeight() / 2);

}

