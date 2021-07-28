#include "../include/NarrowPhaseAlgorithm.h"

using namespace std;

Collision* NarrowPhaseAlgorithm::ballPlane(Point ballCenter, double ballRadius, Point planePoint, Point planeNormal) {
    double d = planeNormal.dotProduct(ballCenter - planePoint);
    double absD= abs(d);
    if (absD < ballRadius) {
        Point normal = (d > 0) ? planeNormal : planeNormal * -1;
        Point projection = ballCenter - normal * (absD * 3 / 2 - ballRadius / 2);
        return new Collision(projection, normal, ballRadius - absD);
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballPlane(Ball* ball, Plane* plane) {
    return ballPlane(ball->getPos(), ball->getRadius(), plane->getPos(), plane->getNormal());
}

Collision* NarrowPhaseAlgorithm::capsulePlane(Capsule* capsule, Plane* plane) {
    Collision* positiveBallCollision = ballPlane(capsule->getCylinderPositiveEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    Collision* negativeBallCollision = ballPlane(capsule->getCylinderNegativeEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    if (positiveBallCollision && negativeBallCollision) return new Collision((positiveBallCollision->getPoint() + negativeBallCollision->getPoint()) / 2, positiveBallCollision->getNormal(), positiveBallCollision->getPenetrationDepth());
    if (positiveBallCollision) return positiveBallCollision;
    return negativeBallCollision;
}


Collision* NarrowPhaseAlgorithm::ballBall(Point center1, double radius1, Point center2, double radius2) {
    Point normalVector = center2 - center1;
    double normalMagnitude = normalVector.getMagnitude();
    double radiusSum = radius1 + radius2;
    if (normalMagnitude < radiusSum) {
        double radiusRatio = radius2 / radiusSum;
        Point collisionPoint = center1 + normalVector * radiusRatio;
        return new Collision(collisionPoint, normalVector / normalMagnitude, radiusSum - normalMagnitude);
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballCylinder(Point ballCenter, double ballRadius, Point cylinderCenter, double cylinderRadius, double cylinderLength, Point cylinderAxisDirection) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Point AB = cylinderAxisDirection;
    Point AP = ballCenter - cylinderCenter;
    Point displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    // Projection of the ball's center is inside the capsule's cylinder
    if (displacementFromA.getMagnitudeSqr() < pow(cylinderLength / 2, 2)) {
        Point projection = cylinderCenter + displacementFromA;
        Point projectionToCenter = ballCenter - projection;
        double distance = projectionToCenter.getMagnitude();
        double radiusSum = ballRadius + cylinderRadius;
        if (distance < radiusSum) {
            Point collisionNormal = projectionToCenter.invert().normalize();
            Point collisionPoint = projection + collisionNormal * cylinderRadius;
            return new Collision(collisionPoint, collisionNormal, radiusSum - distance);
        }
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballBall(Ball* ball1, Ball* ball2) {
    return ballBall(ball1->getPos(), ball1->getRadius(), ball2->getPos(), ball2->getRadius());
}

Collision* NarrowPhaseAlgorithm::ballCapsule(Point ballPos, double ballRadius, Point capsulePos, double capsuleRadius, double capsuleLength, Point capsuleAxisDirection, Point capsulePositiveEnd, Point capsuleNegativeEnd) {
    Collision* cylinderCollision = ballCylinder(ballPos, ballRadius, capsulePos, capsuleRadius, capsuleLength, capsuleAxisDirection);
    if (cylinderCollision) return cylinderCollision;
    Collision* positiveEndCollision = ballBall(ballPos, ballRadius, capsulePositiveEnd, capsuleRadius);
    if (positiveEndCollision) return positiveEndCollision;
    return ballBall(ballPos, ballRadius, capsuleNegativeEnd, capsuleRadius);
}

Collision* NarrowPhaseAlgorithm::ballCapsule(Ball* ball, Capsule* capsule) {
    return ballCapsule(ball->getPos(), ball->getRadius(), capsule->getPos(), capsule->getRadius(), capsule->getLength(), capsule->getAxisDirection(), capsule->getCylinderPositiveEnd(), capsule->getCylinderNegativeEnd());
}

Collision* NarrowPhaseAlgorithm::capsuleCapsule(Capsule* capsule1, Capsule* capsule2) {
    Point UA = capsule1->getAxisDirection();
    Point UB = capsule2->getAxisDirection();

    // Check if capsules are parallel
    if (UA.equals(UB) || UA.equals(UB * -1)) {
        
        Collision* capsule1Ball1 = ballCapsule(capsule1->getCylinderPositiveEnd(), capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
        Collision* capsule1Ball2 = ballCapsule(capsule1->getCylinderNegativeEnd(), capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
        Collision* collision = NULL;

        if (capsule1Ball1 && capsule1Ball2) collision = new Collision((capsule1Ball1->getPoint() + capsule1Ball2->getPoint()) / 2, capsule1Ball1->getNormal(), capsule1Ball1->getPenetrationDepth());
        else {
            Collision* capsule2Ball1 = ballCapsule(capsule2->getCylinderPositiveEnd(), capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());
            if (capsule1Ball1 && capsule2Ball1) collision = new Collision((capsule1Ball1->getPoint() + capsule2Ball1->getPoint()) / 2, capsule1Ball1->getNormal(), capsule1Ball1->getPenetrationDepth());
            else if (capsule1Ball2 && capsule2Ball1) collision = new Collision((capsule1Ball2->getPoint() + capsule2Ball1->getPoint()) / 2, capsule1Ball2->getNormal(), capsule1Ball2->getPenetrationDepth());
            else {
                Collision* capsule2Ball2 = ballCapsule(capsule2->getCylinderNegativeEnd(), capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());
                if (capsule2Ball1 && capsule2Ball2) collision = new Collision((capsule2Ball1->getPoint() + capsule2Ball2->getPoint()) / 2, capsule2Ball1->getNormal(), capsule2Ball1->getPenetrationDepth());
                else if (capsule1Ball1 && capsule2Ball2) collision = new Collision((capsule1Ball1->getPoint() + capsule2Ball2->getPoint()) / 2, capsule1Ball1->getNormal(), capsule1Ball1->getPenetrationDepth());
                else if (capsule1Ball2 && capsule2Ball2) collision = new Collision((capsule1Ball2->getPoint() + capsule2Ball2->getPoint()) / 2, capsule1Ball2->getNormal(), capsule1Ball2->getPenetrationDepth());
                if (capsule2Ball2) delete capsule2Ball2;
            }
            if (capsule2Ball1) delete capsule2Ball1;
        }

        if (capsule1Ball1) delete capsule1Ball1;
        if (capsule1Ball2) delete capsule1Ball2;        

        return collision;
    }

    Point UC = UB.crossProduct(UA).normalize();
    Point RHS = capsule2->getPos() - capsule1->getPos();
    Matrix LHS = Matrix(UA, UB * -1, UC).transpose();

    // https://math.stackexchange.com/questions/1993953/closest-points-between-two-lines
    tuple<double, double, double> solution = solveLinearSystem(LHS, RHS);
    double distanceInAxis1 = get<0>(solution);
    double distanceInAxis2 = get<1>(solution);
    Point axis1Point = UA * distanceInAxis1 + capsule1->getPos();
    Point axis2Point = UB * distanceInAxis2 + capsule2->getPos();
    double distance = abs(get<2>(solution));
    double radiusSum = capsule1->getRadius() + capsule2->getRadius();

    if (distance < radiusSum) {
        // Check collision between cylinders
        if (abs(distanceInAxis1) < capsule1->getLength() / 2 && abs(distanceInAxis2) < capsule2->getLength() / 2) {
            Point normalVector = axis2Point - axis1Point;
            double radiusRatio = capsule2->getRadius() / radiusSum;
            Point collisionPoint = axis1Point + normalVector * radiusRatio;
            return new Collision(collisionPoint, normalVector.normalize(), radiusSum - distance);
        }

        // Check collision between capsule 1 and one of the ends of capsule 2
        Point test1BallCenter = (distanceInAxis2 > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        Collision* collision = ballCylinder(test1BallCenter, capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection());
        if (collision) {
            collision->invertNormal();
            return collision;
        }
        
        // Check collision between capsule 2 and one of the ends of capsule 1
        Point test2BallCenter = (distanceInAxis1 > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        collision =  ballCylinder(test2BallCenter, capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection());
        if (collision) return collision;

        // Check collision between ends of the capsules
        Point ball1Center = (distanceInAxis1 > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        Point ball2Center = (distanceInAxis2 > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        return ballBall(ball1Center, capsule1->getRadius(), ball2Center, capsule2->getRadius());
    }

    return NULL;
}

map<string, Collision> NarrowPhaseAlgorithm::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, Collision> collisions;
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;

        Collision* collision = NULL;

        Ball* ball1 = dynamic_cast<Ball*>(object1);
        Ball* ball2 = dynamic_cast<Ball*>(object2);
        Capsule* capsule1 = dynamic_cast<Capsule*>(object1);
        Capsule* capsule2 = dynamic_cast<Capsule*>(object2);
        Plane* plane1 = dynamic_cast<Plane*>(object1);
        Plane* plane2 = dynamic_cast<Plane*>(object2);

        if (ball1) {
            if (ball2) collision = ballBall(ball1, ball2);
            else if (capsule2) collision = ballCapsule(ball1, capsule2);
            else if (plane2) { collision = ballPlane(ball1, plane2); if (collision) collision->invertNormal(); }
        }
        else if (capsule1) {
            if (ball2) { collision = ballCapsule(ball2, capsule1); if (collision) collision->invertNormal(); }
            else if (capsule2) collision = capsuleCapsule(capsule1, capsule2);
            else if (plane2) { collision = capsulePlane(capsule1, plane2); if (collision) collision->invertNormal(); }
        }
        else if (plane1) {
            if (ball2) collision = ballPlane(ball2, plane1);
            else if (capsule2) collision = capsulePlane(capsule2, plane1);
            else if (plane2) collision = NULL;
        }

        if (collision) {
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(object1, object2);
            
            if (collisions.find(objectPair.first) == collisions.end()) {
                collision->setObjects(objectPair.second);
                auto lastFrameCollision = lastFrameCollisions.find(objectPair.first);
                if (lastFrameCollision != lastFrameCollisions.end()) collision->setLastPenetrationDepth(lastFrameCollision->second.getPenetrationDepth());

                collisions.insert(pair<string, Collision>(objectPair.first, *collision));
            }

            delete collision;
        }
    }

    lastFrameCollisions.clear();
    lastFrameCollisions = collisions;
    return collisions;
}
