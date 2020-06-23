#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() {
}
ofApp::~ofApp() {
}

void ofApp::setup() {

	displayGui = true;
	displayMode = 1; // scientific

	// ##### Load External Elements

	font.load("OpenSans-Light.ttf", 10);

	// ##### Main GUI - Setup

	overlay.setup(); 
	mainGroup.add(overlay.params);
	mainMenu.setup(mainGroup);

	// ##### Info Display

	//infoText.push_back("1,2,3 to select active layer");
	//infoText.push_back("z,x,c,v,.. to start simulations");
	//infoText.push_back("take you somewhere nice");
	//infoText.push_back("Kaspar Ravel");

	// ##### Shading Settings

	setupDeferred();
	panel.add(farClip.set("farClip", 100000., 100., 100000.));
	panel.add(isShowGbuffer.set("show_gbuffer", false));
	panel.add(isShowShadowMap.set("show_shadowMap", false));
	panel.add(isShowDofBuffer.set("show_dofBuffers", false));
	panel.add(isShowBloomBuffer.set("show_bloomBuffers", false));

	// ##### Layer System - Setup

	for (size_t i = 0; i < NUMLAYERS; ++i)
	{
		layers[i] = new Layer(i + 1, Scene_DLA);
		layers[i]->setup();
		layers[i]->inTransition = false;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	// ##### Shading

	cam.setFarClip(farClip);

	// // ##### Camera Light
	shadowLightPass->setGlobalPosition(cam.getGlobalPosition());
	shadowLightPass->lookAt(cam.getLookAtDir());

	// // ##### Sun light
	//shadowLightPass->setGlobalPosition(glm::normalize(glm::vec3(cos(ofGetElapsedTimef() * 0.5), 0.4f + 0.2f * sin(ofGetElapsedTimef() * 0.5f), sin(ofGetElapsedTimef() * 0.5))) * 1600.f);
	//shadowLightPass->lookAt(glm::vec3(0));

	// ##### Layer System - Update

	for (size_t i = 0; i < NUMLAYERS; i++)
	{
		layers[i]->update();
	}
	
	for (auto& ts : transitions) {
		if (!ts.finished && !ts.changed) {
			if (layers[ts.layer]->opacity > 0) {
				layers[ts.layer]->opacity -= 4;
			}
			else if (layers[ts.layer]->opacity <= 0) {
				delete layers[ts.layer];
				layers[ts.layer] = new Layer(ts.layer + 1, ts.type);
				layers[ts.layer]->setup();
				layers[ts.layer]->opacity = 0;
				ts.changed = true;
			}
		}

		if (ts.changed && !ts.finished) {
			if (layers[ts.layer]->opacity < 255) {
				layers[ts.layer]->opacity += 4;
			}
			else if (layers[ts.layer]->opacity >= 255) {
				ts.finished = true;
				layers[ts.layer]->inTransition = false;
				layers[ts.layer]->opacity = 255;
			}
		}
	}
	//deleting all finished transition; it checks if the element is finished, then deletes it and goes to the next, or just goes to the next
	for (vector<LayerTransHelper>::iterator it = transitions.begin(); it != transitions.end();) {
		if (it->finished) {
			it = transitions.erase(it);
		}
		else {
			++it;
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw() {

	// ##### Info Display

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 12, 16);

	// ##### Main GUI - Draw


	overlay.draw();
	mainMenu.draw();																// MAKE ANOTHER FUNCTION FOR THe SCENE MENU DRAW SO THAT I CAN CALL IT BEFORE THE CAMERA
	
	if (displayGui)
	{
		// ##### Info Display					// the INFO DISPLAY CAN GO IN THE OVERLAY ASWELL // set info display to be contrary of bg color

		ofSetColor(255);
		infoText.push_back("Current FPS : " + to_string(ofGetFrameRate()));
		for (size_t i = 0; i < infoText.size(); i++)
		{
			float w = ofGetWidth() - font.stringWidth(infoText[i]) - 10;
			float h = 20 + i * (font.stringHeight(infoText[i]) + 5);
			font.drawString(infoText[i], w, h);
		}
		infoText.pop_back();

		// ##### Display Layer Info

		for (size_t i = 0; i < NUMLAYERS; i++)
		{
			string active = layers[i]->isActiveLayer() ? ">" : "-";
			string text = active + " Layer " + std::to_string(i + 1);
			float w = ofGetWidth() - 55;
			float h = 20 + i * ((font.stringHeight(text)) + 5);
			h += (font.stringHeight(infoText[0])) * infoText.size() + 20;
			font.drawString(text, w, h);
		}
	}

	// ##### DISPLAY MODE : SCIENTIFIC

	if (displayMode == 1)
	{
		cam.begin();
		ofSetBackgroundColor(ofColor::white);
		for (size_t i = 0; i < NUMLAYERS; i++) { layers[i]->draw(); }
		cam.end();
	}

	// ##### DISPLAY MODE : DARK

	else if(displayMode == 2)
	{
		cam.begin();
		ofSetBackgroundColor(ofColor::black);
		for (size_t i = 0; i < NUMLAYERS; i++) { layers[i]->draw(); }
		cam.end();
	}
	// ##### DISPLAY MODE : SHADED

	else
	{
		// Shadow Pass
		shadowLightPass->beginShadowMap(cam);
		for (size_t i = 0; i < NUMLAYERS; i++) { layers[i]->draw(); }
		pointLightPass->drawLights();
		shadowLightPass->endShadowMap();

		// Draw Pass
		deferred.begin(cam);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		for (size_t i = 0; i < NUMLAYERS; i++) { layers[i]->draw(); }
		pointLightPass->drawLights();
		glDisable(GL_CULL_FACE);
		deferred.end();

		// Shading GUI
		if (isShowPanel) {
			if (isShowGbuffer) deferred.debugDraw();
			if (isShowShadowMap) shadowLightPass->debugDraw(glm::vec2(ofGetWidth() - 512, 0), glm::vec2(512));
			if (isShowDofBuffer) dof->debugDraw(glm::vec2(0), glm::vec2(ofGetWidth(), ofGetHeight()) * 0.25f);
			if (isShowBloomBuffer) bloom->debugDraw(glm::vec2(0, ofGetHeight() * 0.25), glm::vec2(ofGetWidth(), ofGetHeight()) * 0.25f);
			panel.draw();
		}
	}

	// ##### GUI - Draw
	
	if (displayGui)
	{
		for (size_t i = 0; i < NUMLAYERS; i++) // draw every layer
		{
			if ((i + 1) == layers[i]->getActiveLayer())
			{
				layers[i]->sceneMenu.draw();
			}
		}
	}
}

void ofApp::setupDeferred() {
	deferred.init(ofGetWidth(), ofGetHeight());

	auto bg = deferred.createPass<ofxDeferred::BgPass>();
	auto e = deferred.createPass<ofxDeferred::EdgePass>();

	deferred.createPass<ofxDeferred::SsaoPass>();
	shadowLightPass = deferred.createPass<ofxDeferred::ShadowLightPass>();
	pointLightPass = deferred.createPass<ofxDeferred::PointLightPass>();

	pointLightPass->addLight();
	//pointLightPass->addLight();

	deferred.createPass<ofxDeferred::FxaaPass>();

	deferred.createPass<ofxDeferred::FogPass>();
	dof = deferred.createPass<ofxDeferred::DofPass>();
	bloom = deferred.createPass<ofxDeferred::BloomPass>();

	panel.setup();
	panel.setDefaultBackgroundColor(ofFloatColor(0., 0.5));
	panel.setDefaultHeaderBackgroundColor(ofFloatColor(0.6, 0.6, 0.3, 0.6));
	panel.setDefaultFillColor(ofFloatColor(0.3, 0.3, 0.6, 0.6));
	panel.add(deferred.getParameters());
	panel.minimizeAll();
	panel.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
	case ' ':
		// used for testing things
		break;
	case 'p':
	{																								// HAVE LITTLE POPUP TEXT SAY IT TOOK SCREENSHOT
		imgScreenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		string filename = "screenshot_" + ofToString(screenshotCount, 5, '0') + ".png";
		imgScreenshot.save(filename);
		screenshotCount++;
		break;
	}
	case 'o':
		displayGui = displayGui ? false : true;
		break;
	case 'q':
		//ofToggleFullscreen();
		//fbo.allocate(ofGetWidth(), ofGetHeight());
		//fbo.begin();
		//ofClear(255);
		//fbo.end();
		break;

	case 'a': initSceneChange(Scene_DiffLine); break;
	case 's': initSceneChange(Scene_DLA); break;
	case 'd': initSceneChange(Scene_SpaceColonization); break;
	case 'f': initSceneChange(Scene_Boids); break;
	case 'g': initSceneChange(Scene_Collatz); break;
	case 'h': initSceneChange(Scene_FluidSimulation); break;
	//case 'j': initSceneChange(Scene_LSystem); break;
	case 'z': initSceneChange(Scene_Julia2D); break;
	case 'x': initSceneChange(Scene_SimplexTerrain); break;
	case 'c': initSceneChange(Scene_DomainWarping); break;
	case 'v': initSceneChange(Scene_Default); break;
	case 'b': initSceneChange(Scene_ShaderTest); break;
	case '1':	layers[0]->setActiveLayer(); break;
	case '2':	layers[1]->setActiveLayer(); break;
	case '3':	layers[2]->setActiveLayer(); break;

	case '7':	displayMode = 1; break; // Display Mode : Scientific
	case '8':	displayMode = 2; break; // Display Mode : Night
	case '9':	displayMode = 3; break; // Display Mode : Shaded


	// shading

	case '!':	isShowPanel = !isShowPanel; break;
	case '@':	pointLightPass->addLight(); break;
		//case '4':	activeLayer = 4; break;
		//case '5':	activeLayer = 5; break;
		//case '6':	activeLayer = 6; break;
		//case '7':	activeLayer = 7; break;
		//case '8':	activeLayer = 8; break;
		//case '9':	activeLayer = 9; break;
	default: break;
	}
}

// ###### Start Scene and Delete Old one Before

void ofApp::startScene(SceneType Type) {                                  // THINK OF SOMEWHERE ELSE TO PUT THIS FUNCTION
	for (size_t i = 0; i < NUMLAYERS; i++)
	{
		if (layers[i]->isActiveLayer())
		{
			delete layers[i];
			layers[i] = new Layer(i + 1, Type);
			layers[i]->setup();

			break;
		}
	}
}

void ofApp::initSceneChange(SceneType Type) {
	for (size_t i = 0; i < NUMLAYERS; i++)
	{
		if (layers[i]->isActiveLayer() && !layers[i]->inTransition)
		{
			LayerTransHelper temp;
			temp.layer = i;
			temp.finished = false;
			temp.changed = false;
			temp.type = Type;

			transitions.push_back(temp);
			layers[i]->inTransition = true;
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	//// # Drag and Drop Sound // Images // Videos
	//if (dragInfo.files.size() > 0)
	//{
	//	dropSound.assign(dragInfo.files.size(), ofSoundPlayer());
	//	for (int s = 0; s < dragInfo.files.size(); s++)
	//	{
	//		dropSound[s].load(dragInfo.files[s]);
	//		dropSound[s].play();
	//		dropSound[s].setVolume(0.6);
	//		dropSound[s].setLoop(true);
	//	}
	//}

	//if (dragInfo.files.size() > 0)
	//{
	//	dropImage.assign(dragInfo.files.size(), ofImage());
	//	for (int i = 0; i < dragInfo.files.size(); i++)
	//	{
	//		dropImage[i].load(dragInfo.files[i]);
	//	}
	//}

	//if (dragInfo.files.size() > 0)
	//{
	//	dropVideo.assign(dragInfo.files.size(), ofVideoPlayer());
	//	for (int v = 0; v < dragInfo.files.size(); v++)
	//	{
	//		dropVideo[v].load(dragInfo.files[v]);
	//		dropVideo[v].play();
	//	}
	//}
}