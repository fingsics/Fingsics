#include "../include/CollisionResponseAlgorithm.h"

void CollisionResponseAlgorithm::moveObjects(Object** objects, int numObjects, float frames, bool slowMotion) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < numObjects; i++) objects[i]->updatePosAndVel(time);
}

void CollisionResponseAlgorithm::calculateNonStaticCollision(Object* object1, Object* object2, Point collisionPoint, Point normal) {
    // https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    Point ra = collisionPoint - object1->getPos();
    Point rb = collisionPoint - object2->getPos();
    Point vai = object1->getVel() + object1->getAngularVelocity().crossProduct(ra);
    Point vbi = object2->getVel() + object2->getAngularVelocity().crossProduct(rb);

    float e = (object1->getElasticity() + object2->getElasticity()) / 2;
    float ma = object1->getMass();
    float mb = object2->getMass();
    Matrix iaInverse = object1->getInertiaTensorInverse();
    Matrix ibInverse = object2->getInertiaTensorInverse();
    Point rxb = rb.crossProduct(normal);
    Point rxa = ra.crossProduct(normal);

    float top = -(1 + e) * (vbi - vai).dotProduct(normal);
    float bottom = 1 / ma + 1 / mb + ((iaInverse * rxa).crossProduct(ra)
        + (ibInverse * rxb).crossProduct(rb)).dotProduct(normal);
    float jr = abs(top / bottom);

    object1->queueImpulse(normal, rxa, -jr, mb);
    object2->queueImpulse(normal, rxb, jr, ma);
}

bool CollisionResponseAlgorithm::handleContact(Object* staticObject, Object* nonStaticObject, Point normal, Point vi, Point vsi) {
    Plane* plane = dynamic_cast<Plane*>(staticObject);
    if (!plane || !normal.hasSameDirection(Point(0, 1, 0), 0.01)) return false;

    Ball* ball = dynamic_cast<Ball*>(nonStaticObject);
    Capsule* capsule = dynamic_cast<Capsule*>(nonStaticObject);
    if (ball) {
        Point vn = normal * normal.dotProduct(vi);
        Point vsn = normal * normal.dotProduct(vsi);
        if ((vn - vsn).getMagnitude() < 1) {
            nonStaticObject->setVel(nonStaticObject->getVel() - vn);
            return true;
        };
    }
    else if (capsule) {
        Point vn = normal * normal.dotProduct(vi);
        Point vsn = normal * normal.dotProduct(vsi);
        if ((vn - vsn).getMagnitude() < 1) {
            if (abs(normal.dotProduct(capsule->getAxisDirection())) < 0.02) {
                nonStaticObject->setAngularVelocity(normal * normal.dotProduct(nonStaticObject->getAngularVelocity()));
                nonStaticObject->setVel(nonStaticObject->getVel() - vn);
                return true;
            }
            else if (abs(normal.dotProduct(capsule->getAxisDirection())) > 0.9) {
                nonStaticObject->setVel(nonStaticObject->getVel() - vn);
                return true;
            }
        };
    }

    return false;
}

void CollisionResponseAlgorithm::calculateStaticCollision(Object* staticObject, Object* nonStaticObject, Point collisionPoint, Point normal) {
    // https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    Point r = collisionPoint - nonStaticObject->getPos();
    Point rs = collisionPoint - staticObject->getPos();
    Point vi = nonStaticObject->getVel() + nonStaticObject->getAngularVelocity().crossProduct(r);
    Point vsi = staticObject->getVel() + staticObject->getAngularVelocity().crossProduct(rs);

    if (handleContact(staticObject, nonStaticObject, normal, vi, vsi)) return;

    float e = (staticObject->getElasticity() + nonStaticObject->getElasticity()) / 2;
    float m = nonStaticObject->getMass();
    Matrix iInverse = nonStaticObject->getInertiaTensorInverse();
    Point rxn = r.crossProduct(normal);

    float top = -(1 + e) * (vi - vsi).dotProduct(normal);
    float bottom = 1 / m + (iInverse * rxn).crossProduct(r).dotProduct(normal);
    float jr = abs(top / bottom);

    nonStaticObject->queueImpulse(normal, rxn, jr, INF);
}

void CollisionResponseAlgorithm::collisionResponse(map<string, Collision> collisionMap) {
    // Calculate per-collision impulses
    for (auto mapEntry : collisionMap) {
        Collision collision = mapEntry.second;
        if (collision.getLastPenetrationDepth() != -1 && collision.getPenetrationDepth() < collision.getLastPenetrationDepth()) continue;
        Object* object1 = collision.getObject1();
        Object* object2 = collision.getObject2();
        Point collisionPoint = collision.getPoint();
        Point collisionNormal = collision.getNormal();
        if (object1->getIsStatic()) calculateStaticCollision(object1, object2, collisionPoint, collisionNormal);
        else if (object2->getIsStatic()) calculateStaticCollision(object2, object1, collisionPoint, collisionNormal * -1);
        else calculateNonStaticCollision(object1, object2, collisionPoint, collisionNormal);
    }

    // Calculate net impulse and apply it
    for (auto mapEntry : collisionMap) {
        Collision collision = mapEntry.second;
        if (collision.getLastPenetrationDepth() != -1 && collision.getPenetrationDepth() < collision.getLastPenetrationDepth()) continue;
        collision.getObject1()->applyQueuedImpulses();
        collision.getObject2()->applyQueuedImpulses();
    }
}