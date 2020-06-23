

#include "ofMain.h"
#include "ofApp.h"

using namespace std;

//========================================================================
int main( ){
	// 4K:4096x2160
	// 2K:2048x1080
	// FullHD:1920x1080
	// HD:1440x1080
	// HD720p:1280x720
	// DVD:720x480

	ofGLWindowSettings settings;
	settings.setGLVersion(4, 3);
	settings.setSize(1920, 1080);

	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}
