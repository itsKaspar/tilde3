#include "Node.h"

Node::Node(glm::vec3 p) {
	pos = p;
	maxForce = ofNoise(p/20);
	maxEdgeLen = ofNoise(p/20) * 50 + 5;
}