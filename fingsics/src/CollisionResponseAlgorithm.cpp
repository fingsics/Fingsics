#include "../include/CollisionResponseAlgorithm.h"

void CollisionResponseAlgorithm::moveObjects(vector<Object*> objects, float framesPerSecond, bool slowMotion) {
    float time = slowMotion ? framesPerSecond / 3 : framesPerSecond;
    for (int i = 0; i < objects.size(); i++) objects[i]->updatePositionAndVelocity(time);
}

void CollisionResponseAlgorithm::calculateNonStaticCollision(Object* object1, Object* object2, Vector collisionPoint, Vector normal) {
    // https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    Vector ra = collisionPoint - object1->getPosition();
    Vector rb = collisionPoint - object2->getPosition();
    Vector vai = object1->getVelocity() + object1->getAngularVelocity().crossProduct(ra);
    Vector vbi = object2->getVelocity() + object2->getAngularVelocity().crossProduct(rb);

    float e = (object1->getElasticity() + object2->getElasticity()) / 2;
    float ma = object1->getMass();
    float mb = object2->getMass();
    Matrix iaInverse = object1->getInertiaTensorInverse();
    Matrix ibInverse = object2->getInertiaTensorInverse();
    Vector rxb = rb.crossProduct(normal);
    Vector rxa = ra.crossProduct(normal);

    float top = -(1 + e) * (vbi - vai).dotProduct(normal);
    float bottom = 1 / ma + 1 / mb + ((iaInverse * rxa).crossProduct(ra)
        + (ibInverse * rxb).crossProduct(rb)).dotProduct(normal);
    float jr = abs(top / bottom);

    object1->queueImpulse(normal, rxa, -jr, mb);
    object2->queueImpulse(normal, rxb, jr, ma);
}

void CollisionResponseAlgorithm::calculateStaticCollision(Object* staticObject, Object* nonStaticObject, Vector collisionPoint, Vector normal) {
    // https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    Vector r = collisionPoint - nonStaticObject->getPosition();
    Vector rs = collisionPoint - staticObject->getPosition();
    Vector vi = nonStaticObject->getVelocity() + nonStaticObject->getAngularVelocity().crossProduct(r);
    Vector vsi = staticObject->getVelocity() + staticObject->getAngularVelocity().crossProduct(rs);

    float e = (staticObject->getElasticity() + nonStaticObject->getElasticity()) / 2;
    float m = nonStaticObject->getMass();
    Matrix iInverse = nonStaticObject->getInertiaTensorInverse();
    Vector rxn = r.crossProduct(normal);

    float top = -(1 + e) * (vi - vsi).dotProduct(normal);
    float bottom = 1 / m + (iInverse * rxn).crossProduct(r).dotProduct(normal);
    float jr = abs(top / bottom);

    nonStaticObject->queueImpulse(normal, rxn, jr, INF);
}

void CollisionResponseAlgorithm::collisionResponse(map<string, Collision> collisionMap) {
    // Calculate per-collision impulses
    for (auto mapEntry : collisionMap) {
        Collision collision = mapEntry.second;
        if (!collision.shouldApplyIt()) continue;
        Object* object1 = collision.getObject1();
        Object* object2 = collision.getObject2();
        Vector collisionPoint = collision.getPoint();
        Vector collisionNormal = collision.getNormal();
        if (object1->getIsStatic()) calculateStaticCollision(object1, object2, collisionPoint, collisionNormal);
        else if (object2->getIsStatic()) calculateStaticCollision(object2, object1, collisionPoint, collisionNormal * -1);
        else calculateNonStaticCollision(object1, object2, collisionPoint, collisionNormal);
    }

    // Calculate net impulse and apply it
    for (auto mapEntry : collisionMap) {
        Collision collision = mapEntry.second;
        if (!collision.shouldApplyIt()) continue;
        collision.getObject1()->applyQueuedImpulses();
        collision.getObject2()->applyQueuedImpulses();
    }
}