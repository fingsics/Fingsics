#include "../include/SceneRecorder.h"

SceneRecorder::SceneRecorder(string path) {
    this->numObjects = -1;
    this->numObjects = -1;
    this->objects = NULL;
    this->positions = NULL;
    this->rotationMatrices = NULL;
    this->path = path;
}

SceneRecorder::SceneRecorder(Object** objects, int numObjects, int frames, string path) {
    this->numObjects = numObjects;
    this->frames = frames;
    this->objects = new SerializedObject[numObjects];
    this->positions = new SerializedPosition*[numObjects];
    this->rotationMatrices = new SerializedMatrix*[numObjects];
    for (int i = 0; i < numObjects; i++) {
        serializeObject(objects[i], &this->objects[i]);
        this->positions[i] = new SerializedPosition[frames];
        this->rotationMatrices[i] = new SerializedMatrix[frames];
    }
    this->path = path;
}

// Serializers

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

void SceneRecorder::serializePosition(Point position, SerializedPosition* serializedPosition) {
    float x = position.getX();
    float y = position.getY();
    float z = position.getZ();
    serializedPosition->x = reinterpret_cast<uint32_t&>(x);
    serializedPosition->y = reinterpret_cast<uint32_t&>(y);
    serializedPosition->z = reinterpret_cast<uint32_t&>(z);
}

void SceneRecorder::serializeRotationMatrix(Matrix rotationMatrix, SerializedMatrix* serializedrotationMatrix) {
    float pos;
    for (int i = 0; i < 9; i++) {
        pos = rotationMatrix[i / 3][i % 3];
        serializedrotationMatrix->values[i] = reinterpret_cast<uint32_t&>(pos);
    }
}

// Deserializers

void SceneRecorder::deserializeObject(SerializedObject serializedObject, SerializedPosition* serializedPositions, SerializedMatrix* serializedRotationMatrices, int frames, string id, Object* object) {
    Color color = Color(serializedObject.r, serializedObject.g, serializedObject.b);
    Point* positions = new Point[frames];
    Matrix* rotationMatrices = new Matrix[frames];

    for (int i = 0; i < frames; i++) {
        deserializePosition(serializedPositions[i], &positions[i]);
        deserializeRotationMatrix(serializedRotationMatrices[i], &rotationMatrices[i]);
    }

    // TODO: Fix lats and longs
    if (serializedObject.type == BALL)
        object = new Ball(id, color, positions, rotationMatrices, frames, reinterpret_cast<float&>(serializedObject.dim1), 5, 5);
    else if (serializedObject.type == CAPSULE)
        object = new Capsule(id, color, positions, rotationMatrices, frames, reinterpret_cast<float&>(serializedObject.dim1), reinterpret_cast<float&>(serializedObject.dim2), 5, 5);
    else if (serializedObject.type == TILE)
        object = new Tile(id, color, positions, rotationMatrices, frames, reinterpret_cast<float&>(serializedObject.dim1), reinterpret_cast<float&>(serializedObject.dim2), serializedObject.draw == DRAW);
}

void SceneRecorder::deserializePosition(SerializedPosition serializedPosition, Point* position) {
    position->setX(reinterpret_cast<float&>(serializedPosition.x));
    position->setY(reinterpret_cast<float&>(serializedPosition.y));
    position->setZ(reinterpret_cast<float&>(serializedPosition.z));
}

void SceneRecorder::recordFrame(Object** objects, int numObjects, int frame) {
    for (int i = 0; i < numObjects; i++) {
        serializePosition(objects[i]->getPosition(), &positions[i][frame]);
        serializeRotationMatrix(objects[i]->getRotationMatrix(), &rotationMatrices[i][frame]);
    }
}

void SceneRecorder::storeRecordedData() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");

    ofstream file("recordings\\" + path, ios::out | ios::binary);
    if (!file) throw "Cannot open file!";

    uint32_t numObjects = this->numObjects;
    uint32_t frames = this->frames;
    file.write((char*)&numObjects, sizeof(uint32_t));
    file.write((char*)&frames, sizeof(uint32_t));

    // Objects
    file.write((char*)&objects, sizeof(SerializedObject) * numObjects);

    // Positions
    for (int i = 0; i < numObjects; i++) {
        file.write((char*)&positions[i], sizeof(SerializedPosition) * frames);
    }
    
    // Rotation matrices
    for (int i = 0; i < numObjects; i++) {
        file.write((char*)&rotationMatrices[i], sizeof(SerializedMatrix) * frames);
    }

    file.close();
    if (!file.good()) throw "Error occurred at writing time!";
}

vector<Object*> SceneRecorder::importRecordedScene() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");
    ifstream file("recordings\\" + path, ios::out | ios::binary);
    if (!file) throw "Cannot open file!";

    uint32_t numObjects;
    uint32_t frames;
    file.read((char*)&numObjects, sizeof(uint32_t));
    file.read((char*)&frames, sizeof(uint32_t));

    SerializedObject* serializedObjects = new SerializedObject[numObjects];
    SerializedPosition** serializedPositions = new SerializedPosition*[numObjects];
    SerializedMatrix** serializedRotationMatrices = new SerializedMatrix*[numObjects];

    // Objects
    file.read((char*)&serializedObjects, sizeof(SerializedObject) * numObjects);

    // Positions
    for (int i = 0; i < numObjects; i++) {
        serializedPositions[i] = new SerializedPosition[frames];
        file.read((char*)&positions[i], sizeof(SerializedPosition) * frames);
    }

    // Rotation matrices
    for (int i = 0; i < numObjects; i++) {
        serializedRotationMatrices[i] = new SerializedMatrix[frames];
        file.read((char*)&rotationMatrices[i], sizeof(SerializedMatrix) * frames);
    }

    // Deserializations
    vector<Object*> res = vector<Object*>(numObjects, NULL);
    for (int i = 0; i < numObjects; i++) {
        deserializeObject(serializedObjects[i], serializedPositions[i], serializedRotationMatrices[i], frames, to_string(i), res[i]);
    }

    file.close();
    if (!file.good()) throw "Error occurred at reading time!";

    delete[] serializedObjects;
    for (int i = 0; i < numObjects; i++) {
        delete[] serializedPositions[i];
        delete[] serializedRotationMatrices[i];
    }
    delete[] serializedPositions;
    delete[] serializedRotationMatrices;

    return res;
}