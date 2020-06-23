
// ###### Particle Systems
// ###### Boids
// ###### Kaspar Ravel - 2020

#include "Boids.h"

void Boids::reset() {
	boids.clear();
	//octree->reset();
	setup();
}

ofParameterGroup Boids::gui() {

	// ##### GUI Setup

	params.setName("Boids");
	params.add(nBoids.set("Boids Qty", 400, 0, 1000));
	params.add(is3D.set("3D", false));
	params.add(alignmentForce.set("Align Force", 1.0, 0.01, 5));
	params.add(cohesionForce.set("Cohes Force", 1.0, 0.01, 5));
	params.add(separationForce.set("Separ Force", 1.0, 0.01, 5));
	params.add(speed.set("Speed", 1.0, 0.01, 50));

	//params.add(drawOctree.set("Draw Octree", false));
	return params;
}

void Boids::setup() {

	//octree = new Octree(ofVec3f(0, 0, 0), ofGetWidth() * 2, true);

	// ##### Boid Spawner

	for (int i = 0; i < nBoids; i++) {

		float x = ofRandom(-ofGetWidth() / 2, ofGetWidth() / 2);
		float y = is3D ? ofRandom(-ofGetWidth() / 2, ofGetWidth() / 2) : ofRandom(-ofGetHeight() / 2, ofGetHeight() / 2);
		float z = is3D ? ofRandom(-ofGetWidth() / 2, ofGetWidth() / 2) : 0;
	//	
		ofVec3f boidPos = ofVec3f(x, y, z);
		boids.push_back(make_shared<Boid>(boidPos));

		//octree->insert(boidPos); // insert into octree
	}

}

void Boids::update() {
	
	for (size_t i = 0; i < boids.size(); i++)
	{
		vector<Particle> neighbours;
		for (size_t j = 0; j < boids.size(); j++)
		{
			float distance = glm::distance(boids[i]->pos, boids[j]->pos);
			if (distance < 150)
			{
				neighbours.push_back(*boids[j]);  // dont i need to make it shared ? or is there just a copy inside of that already ?
			}
		}

		// Make two cases, one where the alignement forces are all the same
		// and one when each boid has a different rate

		//boids[i]->alignment(neighbours, boids[i]->alignmentForce, boids[i]->maxSpeed);
		//boids[i]->cohesion(neighbours, boids[i]->cohesionForce, boids[i]->maxSpeed);
		//boids[i]->separation(neighbours, boids[i]->separationForce, boids[i]->maxSpeed);

		boids[i]->alignment(neighbours, alignmentForce, speed);
		boids[i]->cohesion(neighbours, cohesionForce, speed);
		boids[i]->separation(neighbours, separationForce, speed);

		boids[i]->containment();

		neighbours.clear();
	}
	

	for (size_t i = 0; i < boids.size(); i++)
	{
		boids[i]->update();
		//boids[i].edges();
	}

}

void Boids::draw() {

	for (size_t i = 0; i < boids.size(); i++)
	{
		boids[i]->draw();

	}
}