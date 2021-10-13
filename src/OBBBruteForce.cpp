#include "../include/OBBBruteForce.h"

map<string, pair<Object*, Object*>> OBBBruteForce::getCollisions(vector<Object*> objects) {
    map<string, pair<Object*, Object*>> collisions = map<string, pair<Object*, Object*>>();

    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            if (OBBOverlapTest(objects[i]->getOBB(), objects[j]->getOBB()))
                collisions.insert(getObjectPairWithId(objects[i], objects[j]));
        }
    }

    return collisions;
}

map<string, pair<Object*, Object*>> OBBBruteForce::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, pair<Object*, Object*>> collisions = map<string, pair<Object*, Object*>>();
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;
        if (OBBOverlapTest(object1->getOBB(), object2->getOBB())) collisions.insert(*it);
    }
    return collisions;
}

// Ideas from: http://gamma.cs.unc.edu/SSV/obb.pdf
// Code from: https://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=5
bool OBBBruteForce::OBBOverlapTest(OBB obb1, OBB obb2) {
    Point a = obb1.getHalfLengths();
    Point pos1 = obb1.getPosition();
    Matrix A = obb1.getNormals();

    Point b = obb2.getHalfLengths();
    Point pos2 = obb2.getPosition();
    Matrix B = obb2.getNormals();

    Point v = pos2 - pos1; // T in world's coordenates
    Point T = A * v; // To calculate T in ObjectA's the coordenate system, we use the basis change matrix A
    Matrix R = A * B.transpose(); // ObjectB's rotation relative to ObjectA

    float ra, rb, tl;

    // A faces tests (3)
    for (int i = 0; i < 3; i++) {
        ra = a[i];
        rb = b.dotProduct(R.row(i).abs());
        tl = fabs(T[i]);
        if (tl > ra + rb) return false;
    }

    // B faces tests (3)
    for (int k = 0; k < 3; k++) {
        ra = a.dotProduct(R.col(k).abs());
        rb = b[k];
        tl = fabs(T.dotProduct(R.col(k)));
        if (tl > ra + rb) return false;
    }

    // One axis from each obb tests (9)

    // L = A0 x B0
    ra = a[1] * fabs(R[2][0]) + a[2] * fabs(R[1][0]);
    rb = b[1] * fabs(R[0][2]) + b[2] * fabs(R[0][1]);
    tl = fabs(T[2] * R[1][0] - T[1] * R[2][0]);
    if (tl > ra + rb) return false;

    // L = A0 x B1
    ra = a[1] * fabs(R[2][1]) + a[2] * fabs(R[1][1]);
    rb = b[0] * fabs(R[0][2]) + b[2] * fabs(R[0][0]);
    tl = fabs(T[2] * R[1][1] - T[1] * R[2][1]);
    if (tl > ra + rb) return false;

    // L = A0 x B2
    ra = a[1] * fabs(R[2][2]) + a[2] * fabs(R[1][2]);
    rb = b[0] * fabs(R[0][1]) + b[1] * fabs(R[0][0]);
    tl = fabs(T[2] * R[1][2] - T[1] * R[2][2]);
    if (tl > ra + rb) return false;

    // L = A1 x B0
    ra = a[0] * fabs(R[2][0]) + a[2] * fabs(R[0][0]);
    rb = b[1] * fabs(R[1][2]) + b[2] * fabs(R[1][1]);
    tl = fabs(T[0] * R[2][0] - T[2] * R[0][0]);
    if (tl > ra + rb) return false;

    // L = A1 x B1
    ra = a[0] * fabs(R[2][1]) + a[2] * fabs(R[0][1]);
    rb = b[0] * fabs(R[1][2]) + b[2] * fabs(R[1][0]);
    tl = fabs(T[0] * R[2][1] - T[2] * R[0][1]);
    if (tl > ra + rb) return false;

    // L = A1 x B2
    ra = a[0] * fabs(R[2][2]) + a[2] * fabs(R[0][2]);
    rb = b[0] * fabs(R[1][1]) + b[1] * fabs(R[1][0]);
    tl = fabs(T[0] * R[2][2] - T[2] * R[0][2]);
    if (tl > ra + rb) return false;

    // L = A2 x B0
    ra = a[0] * fabs(R[1][0]) + a[1] * fabs(R[0][0]);
    rb = b[1] * fabs(R[2][2]) + b[2] * fabs(R[2][1]);
    tl = fabs(T[1] * R[0][0] - T[0] * R[1][0]);
    if (tl > ra + rb) return false;

    // L = A2 x B1
    ra = a[0] * fabs(R[1][1]) + a[1] * fabs(R[0][1]);
    rb = b[0] * fabs(R[2][2]) + b[2] * fabs(R[2][0]);
    tl = fabs(T[1] * R[0][1] - T[0] * R[1][1]);
    if (tl > ra + rb) return false;

    // L = A2 x B2
    ra = a[0] * fabs(R[1][2]) + a[1] * fabs(R[0][2]);
    rb = b[0] * fabs(R[2][1]) + b[1] * fabs(R[2][0]);
    tl = fabs(T[1] * R[0][2] - T[0] * R[1][2]);
    if (tl > ra + rb) return false;

    // No separating axis found
    return true;
}