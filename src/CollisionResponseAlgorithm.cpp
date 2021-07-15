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

    double e = (object1->getElasticity() + object2->getElasticity()) / 2;
    double ma = object1->getMass();
    double mb = object2->getMass();
    Matrix iaInverse = object1->getInertiaTensorInverse();
    Matrix ibInverse = object2->getInertiaTensorInverse();

    double top = -(1 + e) * (vbi - vai).dotProduct(normal);
    double bottom = 1 / ma + 1 / mb + ((iaInverse * ra.crossProduct(normal)).crossProduct(ra)
        + (ibInverse * rb.crossProduct(normal)).crossProduct(rb)).dotProduct(normal);
    double jr = abs(top / bottom);

    object1->queueImpulse(normal, ra.crossProduct(normal), -jr, mb);
    object2->queueImpulse(normal, rb.crossProduct(normal), jr, ma);
}

bool CollisionResponseAlgorithm::handleContact(Object* staticObject, Object* nonStaticObject, Point normal, Point vi, Point vsi) {
    Plane* plane = dynamic_cast<Plane*>(staticObject);
    if (plane && normal.hasSameDirection(Point(0, 1, 0), 0.01)) {
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

    double e = (staticObject->getElasticity() + nonStaticObject->getElasticity()) / 2;
    double m = nonStaticObject->getMass();
    Matrix iInverse = nonStaticObject->getInertiaTensorInverse();

    double top = -(1 + e) * (vi - vsi).dotProduct(normal);
    double bottom = 1 / m + (iInverse * r.crossProduct(normal)).crossProduct(r).dotProduct(normal);
    double jr = abs(top / bottom);

    nonStaticObject->queueImpulse(normal, r.crossProduct(normal), jr, INF);
}

void CollisionResponseAlgorithm::collisionResponse(map<string, Collision> collisionMap) {
    // Calculate per-collision impulses
    for (auto mapEntry : collisionMap) {
        if (mapEntry.second.getLastPenetrationDepth() != -1 && mapEntry.second.getPenetrationDepth() < mapEntry.second.getLastPenetrationDepth()) continue;
        Object* object1 = mapEntry.second.getObject1();
        Object* object2 = mapEntry.second.getObject2();
        Point collisionPoint = mapEntry.second.getPoint();
        Point collisionNormal = mapEntry.second.getNormal();
        if (object1->getIsStatic()) calculateStaticCollision(object1, object2, collisionPoint, collisionNormal);
        else if (object2->getIsStatic()) calculateStaticCollision(object2, object1, collisionPoint, collisionNormal * -1);
        else calculateNonStaticCollision(object1, object2, collisionPoint, collisionNormal);
    }

    // Calculate net impulse and apply it
    for (auto mapEntry : collisionMap) {
        if (mapEntry.second.getLastPenetrationDepth() != -1 && mapEntry.second.getPenetrationDepth() < mapEntry.second.getLastPenetrationDepth()) continue;
        Object* object1 = mapEntry.second.getObject1();
        Object* object2 = mapEntry.second.getObject2();
        object1->applyQueuedImpulses();
        object2->applyQueuedImpulses();
    }
}