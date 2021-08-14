#include "../include/NarrowPhaseAlgorithm.h"

using namespace std;

Point average(list<Point*> points) {
    int count = 0;
    Point res = Point();
    list<Point*>::iterator it;
    for (it = points.begin(); it != points.end(); ++it) {
        if (*it) {
            res = res + **it;
            count++;
        }
    }
    return res / count;
}

int sign(float x) {
    return (x > 0) - (x < 0);
}

tuple<Point, Point>* calculateCylinderLineSystemCollision(Capsule* capsule, Point edgeCenter, Point tileAxis, float tileAxisLength, tuple<float, float, float> solution) {
    float distanceInCapsule = get<0>(solution);
    float distanceInEdge = get<1>(solution);
    Point capsulePoint = capsule->getPos() + capsule->getAxisDirection() * (abs(distanceInCapsule) < capsule->getLength() / 2 ? distanceInCapsule : capsule->getLength() * sign(distanceInCapsule) / 2);
    Point axisPoint = edgeCenter + tileAxis * (abs(distanceInEdge) < tileAxisLength / 2 ? distanceInEdge : tileAxisLength * sign(distanceInEdge) / 2);
    float distance = (capsulePoint - axisPoint).getMagnitude();
    return distance < capsule->getRadius() ? new tuple<Point, Point>(axisPoint, capsulePoint) : NULL;
}

// Primitives

tuple<float, float, float> closestPointBetweenNonParallelLines(Point line1Point, Point line1Direction, Point line2Point, Point line2Direction, Point closestPointsDirection) {
    Point UA = line1Direction;
    Point UB = line2Direction;

    Point RHS = line2Point - line1Point;
    Matrix LHS = Matrix(UA, UB * -1, closestPointsDirection).transpose();

    // https://math.stackexchange.com/questions/1993953/closest-points-between-two-lines
    return solveLinearSystem(LHS, RHS);
}

Collision* NarrowPhaseAlgorithm::ballPlane(Point ballCenter, float ballRadius, Point planePoint, Point planeNormal) {
    float d = planeNormal.dotProduct(ballCenter - planePoint);
    float absD = abs(d);
    if (absD < ballRadius) {
        Point normal = (d > 0) ? planeNormal : planeNormal * -1;
        Point projection = ballCenter - normal * (absD * 3 / 2 - ballRadius / 2);
        return new Collision(projection, normal, ballRadius - absD);
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballLine(Point ballCenter, float ballRadius, Point lineCenter, Point lineDirection, float lineLength) {
    float distance = (lineCenter - ballCenter).crossProduct(lineDirection).getMagnitude();
    float absD = abs(distance);
    if (absD < ballRadius) {
        Point projection = lineCenter + lineDirection * (ballCenter - lineCenter).dotProduct(lineDirection);

        if ((projection - lineCenter).getMagnitudeSqr() < pow(lineLength / 2, 2)) {
            return new Collision(projection, (ballCenter - projection).normalize(), ballRadius - absD);
        }
    }

    return NULL;
}

Collision* NarrowPhaseAlgorithm::cylinderLine(Point cylinderCenter, float cylinderRadius, float cylinderLength, Point cylinderAxisDirection, Point lineCenter, Point lineDirection, float lineLength) {
    // TODO
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballBall(Point center1, float radius1, Point center2, float radius2) {
    Point normalVector = center2 - center1;
    float normalMagnitude = normalVector.getMagnitude();
    float radiusSum = radius1 + radius2;
    if (normalMagnitude < radiusSum) {
        float radiusRatio = radius2 / radiusSum;
        Point collisionPoint = center1 + normalVector * radiusRatio;
        return new Collision(collisionPoint, normalVector / normalMagnitude, radiusSum - normalMagnitude);
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballCylinder(Point ballCenter, float ballRadius, Point cylinderCenter, float cylinderRadius, float cylinderLength, Point cylinderAxisDirection) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Point AB = cylinderAxisDirection;
    Point AP = ballCenter - cylinderCenter;
    Point displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    // Projection of the ball's center is inside the capsule's cylinder
    if (displacementFromA.getMagnitudeSqr() < pow(cylinderLength / 2, 2)) {
        Point projection = cylinderCenter + displacementFromA;
        Point projectionToCenter = ballCenter - projection;
        float distance = projectionToCenter.getMagnitude();
        float radiusSum = ballRadius + cylinderRadius;
        if (distance < radiusSum) {
            Point collisionNormal = projectionToCenter.invert().normalize();
            Point collisionPoint = projection + collisionNormal * cylinderRadius;
            return new Collision(collisionPoint, collisionNormal, radiusSum - distance);
        }
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballTile(Point ballCenter, float ballRadius, Point tileCenter, Point tileNormal, Point tileAxis1, Point tileAxis2, float tileAxis1Length, float tileAxis2Length, Point tileEnd1, Point tileEnd2, Point tileEnd3, Point tileEnd4) {
    float d = tileNormal.dotProduct(ballCenter - tileCenter);
    float absD = abs(d);

    if (absD < ballRadius) {
        Point normal = (d > 0) ? tileNormal : tileNormal * -1;
        Point projection = ballCenter - normal * (absD * 3 / 2 - ballRadius / 2);

        float axis1Distance = (tileCenter - projection).crossProduct(tileAxis1).getMagnitude();
        float axis2Distance = (tileCenter - projection).crossProduct(tileAxis2).getMagnitude();

        if (abs(axis1Distance) < tileAxis2Length / 2 && abs(axis2Distance) < tileAxis1Length / 2) {
            // Collision with the plane
            return new Collision(projection, normal, ballRadius - absD);
        }
        else if (abs(axis2Distance) < tileAxis1Length / 2) {
            // Collision with one of the lines
            if ((ballCenter - tileCenter).dotProduct(tileAxis2) < 0) return ballLine(ballCenter, ballRadius, tileCenter - tileAxis2 * tileAxis2Length / 2, tileAxis1, tileAxis1Length);
            else return ballLine(ballCenter, ballRadius, tileCenter + tileAxis2 * tileAxis2Length / 2, tileAxis1, tileAxis1Length);
        }
        else if (abs(axis1Distance) < tileAxis2Length / 2) {
            // Collision with one of the lines
            if ((ballCenter - tileCenter).dotProduct(tileAxis1) < 0) return ballLine(ballCenter, ballRadius, tileCenter - tileAxis1 * tileAxis1Length / 2, tileAxis2, tileAxis2Length);
            else return ballLine(ballCenter, ballRadius, tileCenter + tileAxis1 * tileAxis1Length / 2, tileAxis2, tileAxis2Length);
        }
        else {
            // Test collision with tile's vertices
            Point tileEnd;
            float cos1 = (ballCenter - tileCenter).dotProduct(tileAxis2);
            float cos2 = (ballCenter - tileCenter).dotProduct(tileAxis1);

            if (cos1 > 0 && cos2 > 0) tileEnd = tileEnd1;
            else if (cos1 > 0) tileEnd = tileEnd2;
            else if (cos2 > 0) tileEnd = tileEnd3;
            else tileEnd = tileEnd4;

            Point endToBall = ballCenter - tileEnd;
            float distance = endToBall.getMagnitude();
            if (distance < ballRadius) {
                return new Collision(tileEnd, endToBall / distance, ballRadius - distance);
            }
        }
    }


    return NULL;
}

// Objects

Collision* NarrowPhaseAlgorithm::ballPlane(Ball* ball, Plane* plane) {
    return ballPlane(ball->getPos(), ball->getRadius(), plane->getPos(), plane->getNormal());
}

Collision* NarrowPhaseAlgorithm::ballTile(Ball* ball, Tile* tile) {
    return ballTile(ball->getPos(), ball->getRadius(), tile->getPos(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());
}

Collision* NarrowPhaseAlgorithm::capsuleTile(Capsule* capsule, Tile* tile) {
    Point cylinderPositiveEnd = capsule->getCylinderPositiveEnd();
    Point cylinderNegativeEnd = capsule->getCylinderNegativeEnd();

    Collision* positiveCollision = ballTile(cylinderPositiveEnd, capsule->getRadius(), tile->getPos(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());
    Collision* negativeCollision = ballTile(cylinderNegativeEnd, capsule->getRadius(), tile->getPos(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());

    if (positiveCollision && negativeCollision) return new Collision((positiveCollision->getPoint() + negativeCollision->getPoint()) / 2, (positiveCollision->getNormal() + positiveCollision->getNormal()).normalize(), (positiveCollision->getPenetrationDepth() + negativeCollision->getPenetrationDepth()) / 2);
    if (positiveCollision) return positiveCollision;
    if (negativeCollision) return negativeCollision;
    
    // TODO: Check for parallel axis on top
    
    // TODO: Check for parallel axis on the side

    // Test the edges of the tile, if two report collisions check which collision happens on the tip of the tile, if both do return the tip, if only one does return the other one
    Point UA = capsule->getAxisDirection();
    Point UB12 = tile->getAxis1();
    Point UB34 = tile->getAxis2();

    Point edge1Center = tile->getPos() + UB12 * tile->getAxis1Length() / 2;
    Point edge2Center = tile->getPos() - UB12 * tile->getAxis1Length() / 2;
    Point edge3Center = tile->getPos() + UB34 * tile->getAxis2Length() / 2;
    Point edge4Center = tile->getPos() - UB34 * tile->getAxis2Length() / 2;

    Point capsuleCenter = capsule->getPos();
    Point capsuleAxis= capsule->getAxisDirection();

    Point UC12 = UB12.crossProduct(UA).normalize();
    tuple<float, float, float> edge1Solution = closestPointBetweenNonParallelLines(capsuleCenter, UA, edge1Center, UB34, UC12);
    tuple<float, float, float> edge2Solution = closestPointBetweenNonParallelLines(capsuleCenter, UA, edge2Center, UB34, UC12);
    Point UC34 = UB34.crossProduct(UA).normalize();
    tuple<float, float, float> edge3Solution = closestPointBetweenNonParallelLines(capsuleCenter, UA, edge3Center, UB12, UC34);
    tuple<float, float, float> edge4Solution = closestPointBetweenNonParallelLines(capsuleCenter, UA, edge4Center, UB12, UC34);

    tuple<Point, Point>* edge1Collision = calculateCylinderLineSystemCollision(capsule, edge1Center, UB34, tile->getAxis2Length(), edge1Solution);
    tuple<Point, Point>* edge2Collision = calculateCylinderLineSystemCollision(capsule, edge2Center, UB34, tile->getAxis2Length(), edge2Solution);
    tuple<Point, Point>* edge3Collision = calculateCylinderLineSystemCollision(capsule, edge3Center, UB12, tile->getAxis1Length(), edge3Solution);
    tuple<Point, Point>* edge4Collision = calculateCylinderLineSystemCollision(capsule, edge4Center, UB12, tile->getAxis1Length(), edge4Solution);

    if (!(edge1Collision || edge2Collision || edge3Collision || edge4Collision)) return NULL;

    Point tilePoint = average({ edge1Collision ? &get<0>(*edge1Collision) : NULL, edge2Collision ? &get<0>(*edge2Collision) : NULL, edge3Collision ? &get<0>(*edge3Collision) : NULL, edge4Collision ? &get<0>(*edge4Collision) : NULL });
    Point capsulePoint = average({ edge1Collision ? &get<1>(*edge1Collision) : NULL, edge2Collision ? &get<1>(*edge2Collision) : NULL, edge3Collision ? &get<1>(*edge3Collision) : NULL, edge4Collision ? &get<1>(*edge4Collision) : NULL });

    float penetrationDepth = capsule->getRadius() - (capsulePoint - tilePoint).getMagnitude();


    return new Collision(tilePoint, (tilePoint - capsulePoint).normalize(), penetrationDepth);
}

Collision* NarrowPhaseAlgorithm::capsulePlane(Capsule* capsule, Plane* plane) {
    Collision* positiveBallCollision = ballPlane(capsule->getCylinderPositiveEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    Collision* negativeBallCollision = ballPlane(capsule->getCylinderNegativeEnd(), capsule->getRadius(), plane->getPos(), plane->getNormal());
    if (positiveBallCollision && negativeBallCollision) return new Collision((positiveBallCollision->getPoint() + negativeBallCollision->getPoint()) / 2, positiveBallCollision->getNormal(), positiveBallCollision->getPenetrationDepth());
    return positiveBallCollision ? positiveBallCollision : negativeBallCollision;
}

Collision* NarrowPhaseAlgorithm::ballBall(Ball* ball1, Ball* ball2) {
    return ballBall(ball1->getPos(), ball1->getRadius(), ball2->getPos(), ball2->getRadius());
}

Collision* NarrowPhaseAlgorithm::ballCapsule(Point ballPos, float ballRadius, Point capsulePos, float capsuleRadius, float capsuleLength, Point capsuleAxisDirection, Point capsulePositiveEnd, Point capsuleNegativeEnd) {
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
    tuple<float, float, float> solution = closestPointBetweenNonParallelLines(capsule1->getPos(), UA, capsule2->getPos(), UB, UC);
    
    float distanceInAxis1 = get<0>(solution);
    float distanceInAxis2 = get<1>(solution);
    Point axis1Point = UA * distanceInAxis1 + capsule1->getPos();
    Point axis2Point = UB * distanceInAxis2 + capsule2->getPos();
    float distance = abs(get<2>(solution));
    float radiusSum = capsule1->getRadius() + capsule2->getRadius();

    if (distance < radiusSum) {
        // Check collision between cylinders
        if (abs(distanceInAxis1) < capsule1->getLength() / 2 && abs(distanceInAxis2) < capsule2->getLength() / 2) {
            Point normalVector = axis2Point - axis1Point;
            if (normalVector.isZero()) normalVector = UC;
            float radiusRatio = capsule2->getRadius() / radiusSum;
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
        Tile* tile1 = dynamic_cast<Tile*>(object1);
        Tile* tile2 = dynamic_cast<Tile*>(object2);

        if (ball1) {
            if (ball2) collision = ballBall(ball1, ball2);
            else if (capsule2) collision = ballCapsule(ball1, capsule2);
            else if (plane2) { collision = ballPlane(ball1, plane2); if (collision) collision->invertNormal(); }
            else if (tile2) { collision = ballTile(ball1, tile2); if (collision) collision->invertNormal(); }
        }
        else if (capsule1) {
            if (ball2) { collision = ballCapsule(ball2, capsule1); if (collision) collision->invertNormal(); }
            else if (capsule2) collision = capsuleCapsule(capsule1, capsule2);
            else if (plane2) { collision = capsulePlane(capsule1, plane2); if (collision) collision->invertNormal(); }
            else if (tile2) collision = capsuleTile(capsule1, tile2);
        }
        else if (plane1) {
            if (ball2) collision = ballPlane(ball2, plane1);
            else if (capsule2) collision = capsulePlane(capsule2, plane1);
            else if (plane2) collision = NULL;
            else if (tile2) collision = NULL;
        }
        else if (tile1) {
            if (ball2) collision = ballTile(ball2, tile1);
            else if (capsule2) { collision = capsuleTile(capsule2, tile1); if (collision) collision->invertNormal(); }
            else if (plane2) collision = NULL;
            else if (tile2) collision = NULL;
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
