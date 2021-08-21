#include "../include/SceneRecorder.h"

SceneRecorder::SceneRecorder(Object** objects, int numObjects, int frames, string path) {
    this->currentFrame = 0;
    this->numObjects = numObjects;
    this->numObjects = frames;
    this->objects = new SerializedObject[numObjects];
    this->positions = new SerializedPosition*[numObjects];
    this->rotationMatrices = new SerializedRotationMatrix*[numObjects];
    for (int i = 0; i < numObjects; i++) {
        serializeObject(objects[i], &this->objects[i]);
        this->positions[i] = new SerializedPosition[frames];
        this->rotationMatrices[i] = new SerializedRotationMatrix[frames];
    }
    this->path = path;
}

void SceneRecorder::serializeObject(Object* object, SerializedObject* serializedObject) {
    serializedObject->r = object->getColor().getR();
    serializedObject->g = object->getColor().getG();
    serializedObject->b = object->getColor().getB();
    if (Ball* ball = dynamic_cast<Ball*>(object)) {
        float radius = ball->getRadius();
        serializedObject->type = BALL;
        serializedObject->dim1 = reinterpret_cast<uint32_t&>(radius);
        serializedObject->dim2 = 0;
        serializedObject->draw = 1;
    }
    else if (Capsule* capsule = dynamic_cast<Capsule*>(object)) {
        float radius = capsule->getRadius();
        float length = capsule->getLength();
        serializedObject->type = CAPSULE;
        serializedObject->dim1 = reinterpret_cast<uint32_t&>(radius);
        serializedObject->dim2 = reinterpret_cast<uint32_t&>(length);
        serializedObject->draw = 1;
    }
    else if (Tile* tile = dynamic_cast<Tile*>(object)) {
        float length1 = tile->getAxis1Length();
        float length2 = tile->getAxis2Length();
        serializedObject->type = TILE;
        serializedObject->dim1 = reinterpret_cast<uint32_t&>(length1);
        serializedObject->dim2 = reinterpret_cast<uint32_t&>(length2);
        serializedObject->draw = tile->getDraw();
    }
}

Object* SceneRecorder::deserializeObject(SerializedObject serializedObject, SerializedPosition* serializedPositions, SerializedRotationMatrix* serializedRotationMatrices, int frames, string id) {
    Color color = Color(serializedObject.r, serializedObject.g, serializedObject.b);

    // TODO: Fix positions and rotation matrices
    // TODO: Fix lats and longs
    if (serializedObject.type == BALL) return new Ball(id, color, NULL, NULL, 0, reinterpret_cast<float&>(serializedObject.dim1), 5,5);
    else if (serializedObject.type == CAPSULE) return new Capsule(id, color, NULL, NULL, 0, reinterpret_cast<float&>(serializedObject.dim1), reinterpret_cast<float&>(serializedObject.dim2), 5, 5);
    else if (serializedObject.type == TILE) return new Tile(id, color, NULL, NULL, 0, reinterpret_cast<float&>(serializedObject.dim1), reinterpret_cast<float&>(serializedObject.dim2), serializedObject.draw == DRAW);
}

void SceneRecorder::recordFrame() {
    return;
}

void SceneRecorder::storeRecordedData() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");

    ofstream file("recordings\\" + path, ios::out | ios::binary);
    if (!file) throw "Cannot open file!";

    uint32_t numObjects = this->numObjects;
    uint32_t frames = this->frames;
    file.write((char*)&numObjects, sizeof(uint32_t));
    file.write((char*)&frames, sizeof(uint32_t));

    for (int i = 0; i < numObjects; i++) file.write((char*)&objects[i], sizeof(SerializedObject));

    // TODO: Record positions and matrices

    file.close();
    if (!file.good()) throw "Error occurred at writing time!";
}

vector<Object*> SceneRecorder::importRecordedScene() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");
    ifstream rf("recordings\\" + path, ios::out | ios::binary);
    if (!rf) throw "Cannot open file!";

    uint32_t numObjects;
    uint32_t frames;
    rf.read((char*)&numObjects, sizeof(uint32_t));
    rf.read((char*)&frames, sizeof(uint32_t));

    SerializedObject* incoming = new SerializedObject[numObjects];
    vector<Object*> res = vector<Object*>(numObjects, NULL);

    for (int i = 0; i < numObjects; i++) {
        rf.read((char*)&incoming[i], sizeof(SerializedObject));
    }

    // TODO: Read positions and matrices

    rf.close();
    if (!rf.good()) throw "Error occurred at reading time!";

    return res;
}