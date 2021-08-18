#include "../include/SceneRecorder.h"

void recordScene() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");

    ofstream wf("recordings\\hola.dat", ios::out | ios::binary);
    if (!wf) throw "Cannot open file!";
    const int num = 10;
    float dims[2 * num];
    SerializedObject objects[num];
    for (int i = 0; i < num; i++) {
        objects[i].type = rand() % 3;
        objects[i].r = rand() * 255;
        objects[i].g = rand() * 255;
        objects[i].b = rand() * 255;
        dims[2 * i] = rand() * 1000;
        objects[i].dim1 = reinterpret_cast<uint32_t&>(dims[2 * i]);
        dims[2 * i + 1] = objects[i].type != 0 ? rand() * 1000 : 0;
        objects[i].dim2 = reinterpret_cast<uint32_t&>(dims[2 * i + 1]);
    }
    for (int i = 0; i < num; i++) wf.write((char*)&objects[i], sizeof(SerializedObject));
    wf.close();
    if (!wf.good()) throw "Error occurred at writing time!";
}

void importScene() {
    if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");
    ifstream rf("recordings\\hola.dat", ios::out | ios::binary);
    if (!rf) throw "Cannot open file!";
    const int num = 10;
    float dims2[2 * num];
    SerializedObject objects2[num];
    for (int i = 0; i < num; i++) rf.read((char*)&objects2[i], sizeof(SerializedObject));
    rf.close();
    if (!rf.good()) throw "Error occurred at reading time!";

    for (int i = 0; i < num; i++) {
        dims2[2 * i] = reinterpret_cast<float&>(objects2[i].dim1);
        dims2[2 * i + 1] = reinterpret_cast<float&>(objects2[i].dim2);
    }
}
