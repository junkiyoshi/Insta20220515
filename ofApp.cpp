#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);
	this->word = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

//--------------------------------------------------------------
void ofApp::update() {

	int base_radius = ofMap(ofNoise(ofGetFrameNum() * 0.01), 0, 1, 60, 420);

	for (int i = this->location_list.size() - 1; i >= 0; i--) {

		this->size_list[i] += this->speed_list[i];

		if (this->size_list[i] > this->max_size_list[i]) {

			this->location_list.erase(this->location_list.begin() + i);
			this->size_list.erase(this->size_list.begin() + i);
			this->speed_list.erase(this->speed_list.begin() + i);
			this->max_size_list.erase(this->max_size_list.begin() + i);
			this->index_list.erase(this->index_list.begin() + i);
			this->deg_list.erase(this->deg_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->flag_list.erase(this->flag_list.begin() + i);
		}
	}

	ofColor color;
	for (int i = 0; i < 12; i++) {

		int random_deg = ofRandom(360);
		int random_radius = base_radius + ofRandom(-10, 10);

		auto location = glm::vec2(random_radius * cos(random_deg * DEG_TO_RAD), random_radius * sin(random_deg * DEG_TO_RAD));
		this->location_list.push_back(location);
		this->size_list.push_back(1);
		this->speed_list.push_back(ofRandom(0.4, 2));
		this->max_size_list.push_back(ofRandom(5, 45));
		this->index_list.push_back(ofRandom(this->word.size()));
		this->deg_list.push_back(ofRandom(360));
		color.setHsb(ofRandom(255), 255, 255);
		this->color_list.push_back(color);
		this->flag_list.push_back(ofRandom(1) < 0.5);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);
	ofNoFill();

	for (int i = 0; i < this->location_list.size(); i++) {

		this->flag_list[i] ? ofFill() : ofNoFill();

		ofPath chara_path = this->font.getCharacterAsPoints(this->word[this->index_list[i]], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();
		auto rotation = glm::rotate(glm::mat4(), this->deg_list[i] * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

		ofSetColor(this->color_list[i], ofMap(this->size_list[i], 1, this->max_size_list[i], 255, 30));
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			auto vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				glm::vec2 location = vertex / 100 * size_list[i] * 1.5;
				location -= glm::vec2(size_list[i] * 0.5, -size_list[i] * 0.5);
				location = glm::vec4(location, 0, 0) * rotation;
				ofVertex(location + location_list[i]);
			}
		}
		ofEndShape(true);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}