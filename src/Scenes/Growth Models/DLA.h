
// ###### Growth Model Studies
// ###### Diffusion Limited Aggregation
// ###### Kaspar Ravel - 2020

#pragma once

#ifndef _DLA
#define _DLA

#include "ofMain.h" 
#include "ofxGui.h"
#include "Scene.h"
#include "RandomWalker.h"
#include "Octree.h"

class DLA : public Scene
{
public:

	// ##### Basic Functions
	//DLA();
	//~DLA();
	void reset();
	void setup();
	void update();
	void draw();

	// ##### GUI Setup

	ofParameterGroup gui();
	ofParameterGroup params;
	ofParameter<int> xSteps;
	ofParameter<bool> isCentered;
	ofParameter<bool> is3D;
	ofParameter<bool> drawOctree;
	ofParameter<float> sticky;
	ofParameter<float> towardsAggregation;
	ofParameter<int> nWalkers;
	ofParameter<float> sWalkers;
	ofParameter<float> walkerRadius;
	ofParameter<bool> displayWalkers;

	// ##### Layer Settings
	
	void setColor1(ofColor color1) { c1 = color1; }
	void setColor2(ofColor color2) { c2 = color2; }
	void setOpacity(int o) { opacity = o; }

	ofColor c1;
	ofColor c2;
	int opacity;

	// ##### Particle Vectors

	vector<std::shared_ptr<RandomWalker>> walkers;
	vector<std::shared_ptr<RandomWalker>> fixed;

	// ##### Other Variables

	int walkerQty;
	float walkerWalk;

	Octree *octree;

	// ##### Compute Shader

	static const int WORK_GROUP_SIZE = 512;
	int mNumParticles;
	ofShader mComputeShader;

private:
};

#endif


