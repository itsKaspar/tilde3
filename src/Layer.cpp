
// ##### Layer Class
// #####
// ##### The Layer class is parent to every class in the Scenes folder
// ##### It creates a sort of wrapper with parameters common to every scene
// ##### Things like opacity, color palette, etc

#include "Layer.h"
#include "Global.h"

// ##### Scene Headers Includes

#include "DefaultScene.h"
#include "DLA.h"
#include "Diffline.h"
#include "SpaceColonization.h"
#include "Collatz.h"
//#include "LSystem.h"

#include "Boids.h"
#include "FluidSimulation.h"

#include "SimplexTerrain.h"
#include "Julia2D.h"
#include "DomainWarping.h"

#include "ShaderTest.h"
#include "Test1.h"

#include "Sunset.h"

int Layer::activeLayer;

Layer::Layer() {
}

Layer::Layer(int i, SceneType Type) {

	// ##### Layer Variables 

	id = i;
	activeLayer = i;

	// ##### Time Handling

	bpm = 120;
	timeSinceLastBeat = 0;

	// ##### Scene Initiation

	scene = CreateScene(Type);

}

Layer::~Layer() {

	delete scene;
}


void Layer::setup() {

	gui();

	scene->setup();

	reset.addListener(this, &Layer::resetChanged);
}

void Layer::resetChanged(bool &reset) {

	scene->reset();
}


void Layer::gui()
{
	// ##### GUI Setup
	
	params.setName("Scene Settings");                                       // ADD THIS NAME OF SIMULATION
	params.add(opacity.set("Opacity", 255, 0, 255));

	params.add(c1.set(ofColor(200, 100, 148)));
	params.add(c2.set(ofColor(19, 140, 215)));
	params.add(speedCamera.set("Rotation", ofVec3f::zero(), ofVec3f::zero(), ofVec3f(100)));
	params.add(resetAtBpm.set("BPM follow", false));
	params.add(xBpm.set("BPM Multiplier", 1, 0.001, 8.0));
	params.add(reset.set("Reset", false));

	sceneGroup.add(params); // add general scene menu
	sceneGroup.add(scene->gui()); // add specific scene menu
	sceneMenu.setup(sceneGroup);
}

void Layer::update() {

	// ##### BPM Follow System

	if (resetAtBpm)
	{
		float bps = 1 / (float)((bpm*xBpm) / 60); // calculate beats per seconds
		timeSinceLastBeat += ofGetLastFrameTime();
		if (timeSinceLastBeat > bps)
		{
			scene->reset();
			timeSinceLastBeat = 0;
		}
	}

	//// ##### Update

	scene->update();

	//// ##### Pass Color to Scene

	scene->setColor1(c1);
	scene->setColor2(c2);
	scene->setOpacity(opacity);
}


void Layer::draw() {
	if (opacity > 0) // if opacity of the layer is at 0 don't draw
	{

	// ##### Rotate Camera View

		ofRotateXDeg(speedCamera->x * ofGetElapsedTimef());
		ofRotateYDeg(speedCamera->y * ofGetElapsedTimef());
		ofRotateZDeg(speedCamera->z * ofGetElapsedTimef());

	// ##### Scene Draw

		scene->draw();
	}
}

// Active Layer Handling
void Layer::setActiveLayer() { activeLayer = id; }
int Layer::getActiveLayer() { return activeLayer; }
bool Layer::isActiveLayer() { if (id == activeLayer) { return true; } else { return false; } }                  // Make this ternaryyyyyyyyyyyyy i can never remember


Scene *Layer::CreateScene(SceneType Type)
{

	switch (Type)
	{
	// ##### Defaults

	case Scene_Default:
	default:
		return new DefaultScene();
	case Scene_None:
		return nullptr;

	// ##### Growth Models

	case Scene_DLA:
		return new DLA();
	case Scene_DiffLine:
		return new DiffLine();
	case Scene_SpaceColonization:
		return new SpaceColonization();
	case Scene_Collatz:
		return new Collatz();
	//case Scene_LSystem:
		//return new LSystem();

	// ##### Particle Systems

	case Scene_Boids:
		return new Boids();
	case Scene_FluidSimulation:
		return new FluidSimulation();

	// ##### Shader Experimentations

	case Scene_ShaderTest:
		return new ShaderTest();
	case Scene_Julia2D:
		return new Julia2D();
	case Scene_SimplexTerrain:
		return new SimplexTerrain();
	case Scene_DomainWarping:
		return new DomainWarping();
	case Scene_Test1:
		return new Test1();

	case Scene_Sunset:
		return new Sunset();
	}
}
