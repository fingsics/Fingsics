#include "../include/NarrowPhaseAlgorithms.h"

using namespace std;

pair<Point, Point>* NarrowPhaseAlgorithms::ballPlane(Point ballCenter, double ballRadius, Point planePoint, Point planeNormal) {
    double d = planeNormal.dotProduct(ballCenter - planePoint);
    if (d < ballRadius) {
        Point projection = ballCenter - planeNormal * d;
        return new pair<Point, Point>(projection, planeNormal);
    }
    return NULL;
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballPlane(Ball* ball, Plane* plane) {
    return ballPlane(ball->getPos(), ball->getRadius(), plane->getPos(), plane->getNormal());
}

pair<Point, Point>* NarrowPhaseAlgorithms::capsulePlane(Capsule* capsule, Plane* plane) {
    pair<Point, Point>* positiveBallCollision = ballPlane(capsule->getCylinderPositiveEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    pair<Point, Point>* negativeBallCollision = ballPlane(capsule->getCylinderNegativeEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    if (positiveBallCollision && negativeBallCollision) return new pair<Point, Point>((positiveBallCollision->first + negativeBallCollision->first) / 2, positiveBallCollision->second);
    if (positiveBallCollision) return positiveBallCollision;
    return negativeBallCollision;
}


pair<Point, Point>* NarrowPhaseAlgorithms::ballBall(Point center1, double radius1, Point center2, double radius2) {
    Point normalVector = center2 - center1;
    if (normalVector.getMagnitude() < radius1 + radius2) {
        double radiusRatio = radius2 / (radius1 + radius2);
        Point collisionPoint = center1 + normalVector * radiusRatio;
        return new pair<Point, Point>(collisionPoint, normalVector.normalize());
    }
    return NULL;
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballCylinder(Point ballCenter, double ballRadius, Point cylinderCenter, double cylinderRadius, double cylinderLength, Point cylinderAxisDirection) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Point AB = cylinderAxisDirection;
    Point AP = ballCenter - cylinderCenter;
    Point displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    // Projection of the ball's center is inside the capsule's cylinder
    if (displacementFromA.getMagnitude() < cylinderLength / 2) {
        Point projection = cylinderCenter + displacementFromA;
        if ((ballCenter - projection).getMagnitude() < ballRadius + cylinderRadius) {
            Point collisionDirection = ballCenter - projection;
            Point collisionPoint = projection + (collisionDirection.normalize() * cylinderRadius);
            Point collisionNormal = projection - collisionPoint;
            return new pair<Point, Point>(collisionPoint, collisionNormal.normalize());
        }
    }
    return NULL;
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballBall(Ball* ball1, Ball* ball2) {
    return ballBall(ball1->getPos(), ball1->getRadius(), ball2->getPos(), ball2->getRadius());
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballCapsule(Point ballPos, double ballRadius, Point capsulePos, double capsuleRadius, double capsuleLength, Point capsuleAxisDirection, Point capsulePositiveEnd, Point capsuleNegativeEnd) {
    pair<Point, Point>* cylinderCollision = ballCylinder(ballPos, ballRadius, capsulePos, capsuleRadius, capsuleLength, capsuleAxisDirection);
    if (cylinderCollision) return cylinderCollision;
    pair<Point, Point>* positiveEndCollision = ballBall(ballPos, ballRadius, capsulePositiveEnd, capsuleRadius);
    if (positiveEndCollision) return positiveEndCollision;
    return ballBall(ballPos, ballRadius, capsuleNegativeEnd, capsuleRadius);
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballCapsule(Ball* ball, Capsule* capsule) {
    return ballCapsule(ball->getPos(), ball->getRadius(), capsule->getPos(), capsule->getRadius(), capsule->getLength(), capsule->getAxisDirection(), capsule->getCylinderPositiveEnd(), capsule->getCylinderNegativeEnd());
}

pair<Point, Point>* NarrowPhaseAlgorithms::capsuleCapsule(Capsule* capsule1, Capsule* capsule2) {
    Point UA = capsule1->getAxisDirection();
    Point UB = capsule2->getAxisDirection();

    // Check if capsules are parallel
    if (UA.equals(UB) || UA.equals(UB * -1)) {
        
        pair<Point, Point>* capsule1Ball1 = ballCapsule(capsule1->getCylinderPositiveEnd(), capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
        pair<Point, Point>* capsule1Ball2 = ballCapsule(capsule1->getCylinderNegativeEnd(), capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
        
        if (capsule1Ball1 && capsule1Ball2) {
            return new pair<Point, Point>((capsule1Ball1->first + capsule1Ball2->first) / 2, capsule1Ball1->second);
        }

        pair<Point, Point>* capsule2Ball1 = ballCapsule(capsule2->getCylinderPositiveEnd(), capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());

        if (capsule1Ball1 && capsule2Ball1) {
            return new pair<Point, Point>((capsule1Ball1->first + capsule2Ball1->first) / 2, capsule1Ball1->second);
        }
        if (capsule1Ball2 && capsule2Ball1) {
            return new pair<Point, Point>((capsule1Ball2->first + capsule2Ball1->first) / 2, capsule1Ball2->second);
        }

        pair<Point, Point>* capsule2Ball2 = ballCapsule(capsule2->getCylinderNegativeEnd(), capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());

        if (capsule2Ball1 && capsule2Ball2) {
            return new pair<Point, Point>((capsule2Ball1->first + capsule2Ball2->first) / 2, capsule2Ball1->second);
        }
        if (capsule1Ball1 && capsule2Ball2) {
            return new pair<Point, Point>((capsule1Ball1->first + capsule2Ball2->first) / 2, capsule1Ball1->second);
        }
        if (capsule1Ball2 && capsule2Ball2) {
            return new pair<Point, Point>((capsule1Ball2->first + capsule2Ball2->first) / 2, capsule1Ball2->second);
        }

        return NULL;
    }

    Point UC = UB.crossProduct(UA).normalize();
    Point RHS = capsule2->getPos() - capsule1->getPos();
    Matrix LHS = Matrix(UA, UB * -1, UC).transpose();

    // https://math.stackexchange.com/questions/1993953/closest-points-between-two-lines
    double* solution = solveLinearSystem(LHS, RHS);
    Point puntoLinea1 = UA * solution[0] + capsule1->getPos();
    Point puntoLinea2 = UB * solution[1] + capsule2->getPos();
    double distancia = abs(solution[2]);

    if (distancia < capsule1->getRadius() + capsule2->getRadius()) {
        // Check collision between cylinders
        if (abs(solution[0]) < capsule1->getLength() / 2 && abs(solution[1]) < capsule2->getLength() / 2) {
            Point normalVector = puntoLinea2 - puntoLinea1;
            double radiusRatio = capsule2->getRadius() / (capsule1->getRadius() + capsule2->getRadius());
            Point collisionPoint = puntoLinea1 + normalVector * radiusRatio;
            return new pair<Point, Point>(collisionPoint, normalVector.normalize());
        }

        // Check collision between capsule 1 and one of the ends of capsule 2
        Point test1BallCenter = (solution[1] > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        pair<Point, Point>* collision = ballCylinder(test1BallCenter, capsule2->getRadius(), capsule1->getPos(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection());
        if (collision) return new pair<Point, Point>(collision->first, collision->second * -1);
        
        // Check collision between capsule 2 and one of the ends of capsule 1
        Point test2BallCenter = (solution[0] > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        collision =  ballCylinder(test2BallCenter, capsule1->getRadius(), capsule2->getPos(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection());
        if (collision) return collision;

        // Check collision between ends of the capsules
        Point ball1Center = (solution[0] > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        Point ball2Center = (solution[1] > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        return ballBall(ball1Center, capsule1->getRadius(), ball2Center, capsule2->getRadius());
    }

    return NULL;
}

map<string, tuple<Object*, Object*, Point, Point>> NarrowPhaseAlgorithms::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, tuple<Object*, Object*, Point, Point>> collisionMap;
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;

        pair<Point, Point>* collision = NULL;

        Ball* ball1 = dynamic_cast<Ball*>(object1);
        Ball* ball2 = dynamic_cast<Ball*>(object2);
        Capsule* capsule1 = dynamic_cast<Capsule*>(object1);
        Capsule* capsule2 = dynamic_cast<Capsule*>(object2);
        Plane* plane1 = dynamic_cast<Plane*>(object1);
        Plane* plane2 = dynamic_cast<Plane*>(object2);

        if (ball1) {
            if (ball2) collision = ballBall(ball1, ball2);
            if (capsule2) collision = ballCapsule(ball1, capsule2);
            if (plane2) collision = ballPlane(ball1, plane2);
        }
        if (capsule1) {
            if (ball2) { collision = ballCapsule(ball2, capsule1); if (collision) collision = new pair<Point, Point>(collision->first, collision->second * -1); }
            if (capsule2) collision = capsuleCapsule(capsule1, capsule2);
            if (plane2) collision = capsulePlane(capsule1, plane2);
        }
        if (plane1) {
            if (ball2) collision = ballPlane(ball2, plane1);
            if (capsule2) collision = capsulePlane(capsule2, plane1);
            if (plane2) collision = NULL;
        }

        if (collision) {
            tuple<Object*, Object*, Point, Point> objectTuple;
            get<0>(objectTuple) = object1;
            get<1>(objectTuple) = object2;
            get<2>(objectTuple) = collision->first;
            get<3>(objectTuple) = collision->second;
            string objectPairId = getObjectPairId(make_pair(object1, object2));

            if (collisionMap.find(objectPairId) == collisionMap.end()) {
                collisionMap.insert(pair<string, tuple<Object*, Object*, Point, Point>>(objectPairId, objectTuple));
            }

            delete collision;
        }
    }

    return collisionMap;
}
