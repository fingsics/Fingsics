#include "../include/MidPhaseAlgorithms.h"

using namespace std;

map<string, pair<Object*, Object*>> NoMidPhase::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    return possibleCollisions;
}

map<string, pair<Object*, Object*>> OBBMidPhase::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, pair<Object*, Object*>> obbCollisions = map<string, pair<Object*, Object*>>();
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;
        if (OBBOverlapTest(object1->getOBB(), object2->getOBB())) obbCollisions.insert(*it);
    }
    return obbCollisions;
}

// http://gamma.cs.unc.edu/SSV/obb.pdf
bool OBBMidPhase::OBBOverlapTest(OBB obb1, OBB obb2) {
    Point a = obb1.getHalfLengths();
    Point pos1 = obb1.getPosition();
    Matrix A = obb1.getRotatedNormals();

    Point b = obb2.getHalfLengths();
    Point pos2 = obb2.getPosition();
    Matrix B = obb2.getRotatedNormals();

    Point v = pos2 - pos1; // T in world's coordenates
    Point T = A * v; // To calculate T in ObjectA's the coordenate system, we use the basis change matrix A
    Matrix R = A * B.transpose(); // ObjectB's rotation relative to ObjectA

    Point L;
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
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            L = A.row(i).crossProduct(B.row(k));
            tl = fabs(T.dotProduct(L));
            ra = fabs((A.row(0) * a[0]).dotProduct(L)) + fabs((A.row(1) * a[1]).dotProduct(L)) + fabs((A.row(2) * a[2]).dotProduct(L));
            rb = fabs((B.row(0) * b[0]).dotProduct(L)) + fabs((B.row(1) * b[1]).dotProduct(L)) + fabs((B.row(2) * b[2]).dotProduct(L));
            if (tl > ra + rb) return false;
        }
    }

    // No separating axis found
    return true;
}

