#include "../include/NarrowPhaseAlgorithm.h"

using namespace std;



Collision* NarrowPhaseAlgorithm::parallelCapsules(Capsule* capsule1, Capsule* capsule2) {
    // Tests all 4 balls with the other capsule 
    Collision* capsule1Ball1 = ballCapsule(capsule1->getCylinderPositiveEnd(), capsule1->getRadius(), capsule2->getPosition(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
    Collision* capsule1Ball2 = ballCapsule(capsule1->getCylinderNegativeEnd(), capsule1->getRadius(), capsule2->getPosition(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection(), capsule2->getCylinderPositiveEnd(), capsule2->getCylinderNegativeEnd());
    Collision* collision = NULL;

    if (capsule1Ball1 && capsule1Ball2) collision = new Collision((capsule1Ball1->getPoint() + capsule1Ball2->getPoint()) / 2, capsule1Ball1->getNormal(), capsule1Ball1->getPenetrationDepth());
    else {
        Collision* capsule2Ball1 = ballCapsule(capsule2->getCylinderPositiveEnd(), capsule2->getRadius(), capsule1->getPosition(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());
        if (capsule1Ball1 && capsule2Ball1) collision = new Collision((capsule1Ball1->getPoint() + capsule2Ball1->getPoint()) / 2, capsule1Ball1->getNormal(), capsule1Ball1->getPenetrationDepth());
        else if (capsule1Ball2 && capsule2Ball1) collision = new Collision((capsule1Ball2->getPoint() + capsule2Ball1->getPoint()) / 2, capsule1Ball2->getNormal(), capsule1Ball2->getPenetrationDepth());
        else {
            Collision* capsule2Ball2 = ballCapsule(capsule2->getCylinderNegativeEnd(), capsule2->getRadius(), capsule1->getPosition(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection(), capsule1->getCylinderPositiveEnd(), capsule1->getCylinderNegativeEnd());
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

tuple<Vector, Vector>* NarrowPhaseAlgorithm::calculateCylinderLineCollision(Capsule* capsule, Vector edgeCenter, Vector tileAxis, float tileAxisLength, tuple<float, float, float> solution) {
    // Returns the collision points on the line and the cylinder axis
    float distanceInCapsule = get<0>(solution);
    float distanceInEdge = get<1>(solution);
    Vector capsulePoint = capsule->getPosition() + capsule->getAxisDirection() * (abs(distanceInCapsule) < capsule->getLength() / 2 ? distanceInCapsule : capsule->getLength() * sign(distanceInCapsule) / 2);
    Vector axisPoint = edgeCenter + tileAxis * (abs(distanceInEdge) < tileAxisLength / 2 ? distanceInEdge : tileAxisLength * sign(distanceInEdge) / 2);
    float distance = (capsulePoint - axisPoint).getMagnitude();
    return distance < capsule->getRadius() ? new tuple<Vector, Vector>(axisPoint, capsulePoint) : NULL;
}

tuple<float, float, float> NarrowPhaseAlgorithm::closestPointBetweenNonParallelLines(Vector line1Point, Vector line1Direction, Vector line2Point, Vector line2Direction, Vector closestPointsDirection) {
    // https://math.stackexchange.com/questions/1993953/closest-points-between-two-lines
    Vector RHS = line2Point - line1Point;
    Matrix LHS = Matrix(line1Direction, line2Direction * -1, closestPointsDirection).transpose();

    return solveLinearSystem(LHS, RHS);
}

tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*> NarrowPhaseAlgorithm::parallelCapsuleAndTileEdgeCollisions(Vector vertex1, Vector vertex2, Vector capsuleCenter, Vector capsuleAxis, float capsuleLength) {
    // Returns the collision points on both ends of the tile edge vertex1-vertex2 and the cylinder axis (if they exist)
    tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*> res = tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*>(NULL, NULL);

    Vector projection1 = capsuleCenter + capsuleAxis * vertex1.dotProduct(capsuleAxis) / capsuleAxis.dotProduct(capsuleAxis);
    Vector projection2 = capsuleCenter + capsuleAxis * vertex2.dotProduct(capsuleAxis) / capsuleAxis.dotProduct(capsuleAxis);

    if ((projection1 - capsuleCenter).getMagnitudeSqr() < pow(capsuleLength / 2, 2)) get<0>(res) = new tuple<Vector, Vector>(vertex1, projection1);
    if ((projection2 - capsuleCenter).getMagnitudeSqr() < pow(capsuleLength / 2, 2)) get<1>(res) = new tuple<Vector, Vector>(vertex2, projection2);

    return res;
}

// Primitives

Collision* NarrowPhaseAlgorithm::ballLine(Vector ballCenter, float ballRadius, Vector lineCenter, Vector lineDirection, float lineLength) {
    float distance = abs((lineCenter - ballCenter).crossProduct(lineDirection).getMagnitude());
    if (distance < ballRadius) {
        Vector projection = lineCenter + lineDirection * (ballCenter - lineCenter).dotProduct(lineDirection);

        if ((projection - lineCenter).getMagnitudeSqr() < pow(lineLength / 2, 2)) {
            return new Collision(projection, (ballCenter - projection).normalize(), ballRadius - distance);
        }
    }

    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballBall(Vector center1, float radius1, Vector center2, float radius2) {
    Vector normalVector = center2 - center1;
    float normalMagnitude = normalVector.getMagnitude();
    float radiusSum = radius1 + radius2;
    if (normalMagnitude < radiusSum) {
        float radiusRatio = radius2 / radiusSum;
        Vector collisionPoint = center1 + normalVector * radiusRatio;
        return new Collision(collisionPoint, normalVector / normalMagnitude, radiusSum - normalMagnitude);
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballCylinder(Vector ballCenter, float ballRadius, Vector cylinderCenter, float cylinderRadius, float cylinderLength, Vector cylinderAxisDirection) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Vector AB = cylinderAxisDirection;
    Vector AP = ballCenter - cylinderCenter;
    Vector displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    // Projection of the ball's center is inside the capsule's cylinder
    if (displacementFromA.getMagnitudeSqr() < pow(cylinderLength / 2, 2)) {
        Vector projection = cylinderCenter + displacementFromA;
        Vector projectionToCenter = ballCenter - projection;
        float distance = projectionToCenter.getMagnitude();
        float radiusSum = ballRadius + cylinderRadius;
        if (distance < radiusSum) {
            Vector collisionNormal = projectionToCenter.invert().normalize();
            Vector collisionPoint = projection + collisionNormal * cylinderRadius;
            return new Collision(collisionPoint, collisionNormal, radiusSum - distance);
        }
    }
    return NULL;
}

Collision* NarrowPhaseAlgorithm::ballTile(Vector ballCenter, float ballRadius, Vector tileCenter, Vector tileNormal, Vector tileAxis1, Vector tileAxis2, float tileAxis1Length, float tileAxis2Length, Vector tileEnd1, Vector tileEnd2, Vector tileEnd3, Vector tileEnd4) {
    float d = tileNormal.dotProduct(ballCenter - tileCenter);
    float absD = abs(d);

    if (absD < ballRadius) {
        Vector normal = (d > 0) ? tileNormal : tileNormal * -1;
        Vector projection = ballCenter - normal * (absD * 3 / 2 - ballRadius / 2);

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
            Vector tileEnd;
            float cos1 = (ballCenter - tileCenter).dotProduct(tileAxis2);
            float cos2 = (ballCenter - tileCenter).dotProduct(tileAxis1);

            if (cos1 > 0 && cos2 > 0) tileEnd = tileEnd1;
            else if (cos1 > 0) tileEnd = tileEnd2;
            else if (cos2 > 0) tileEnd = tileEnd3;
            else tileEnd = tileEnd4;

            Vector endToBall = ballCenter - tileEnd;
            float distance = endToBall.getMagnitude();
            if (distance < ballRadius) {
                return new Collision(tileEnd, endToBall / distance, ballRadius - distance);
            }
        }
    }


    return NULL;
}

// Objects

Collision* NarrowPhaseAlgorithm::ballTile(Ball* ball, Tile* tile) {
    return ballTile(ball->getPosition(), ball->getRadius(), tile->getPosition(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());
}

Collision* NarrowPhaseAlgorithm::capsuleTile(Capsule* capsule, Tile* tile) {
    Vector cylinderPositiveEnd = capsule->getCylinderPositiveEnd();
    Vector cylinderNegativeEnd = capsule->getCylinderNegativeEnd();

    Collision* positiveCollision = ballTile(cylinderPositiveEnd, capsule->getRadius(), tile->getPosition(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());
    Collision* negativeCollision = ballTile(cylinderNegativeEnd, capsule->getRadius(), tile->getPosition(), tile->getNormal(), tile->getAxis1(), tile->getAxis2(), tile->getAxis1Length(), tile->getAxis2Length(), tile->getEnd1(), tile->getEnd2(), tile->getEnd3(), tile->getEnd4());

    if (positiveCollision && negativeCollision) {
        Collision* res = new Collision((positiveCollision->getPoint() + negativeCollision->getPoint()) / 2, (positiveCollision->getNormal() + positiveCollision->getNormal()).normalize(), (positiveCollision->getPenetrationDepth() + negativeCollision->getPenetrationDepth()) / 2);
        delete positiveCollision;
        delete negativeCollision;
        return res;
    }

    Vector UA = capsule->getAxisDirection();
    Vector UB12 = tile->getAxis1();
    Vector UB34 = tile->getAxis2();

    Vector edge1Center = tile->getPosition() + UB12 * tile->getAxis1Length() / 2;
    Vector edge2Center = tile->getPosition() - UB12 * tile->getAxis1Length() / 2;
    Vector edge3Center = tile->getPosition() + UB34 * tile->getAxis2Length() / 2;
    Vector edge4Center = tile->getPosition() - UB34 * tile->getAxis2Length() / 2;

    tuple<Vector, Vector>* collision1 = NULL;
    tuple<Vector, Vector>* collision2 = NULL;
    tuple<Vector, Vector>* collision3 = NULL;
    tuple<Vector, Vector>* collision4 = NULL;

    if (capsule->getAxisDirection().parallel(UB34)) {
        // The capsule is parallel to two of the plane's edges
        tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*> collisions;
        if (UB34.crossProduct(capsule->getPosition() - edge1Center).getMagnitudeSqr() < pow(capsule->getRadius(), 2)) {
            collisions = parallelCapsuleAndTileEdgeCollisions(tile->getEnd1(), tile->getEnd3(), capsule->getPosition(), capsule->getAxisDirection(), capsule->getLength());
            collision1 = get<0>(collisions);
            collision2 = get<1>(collisions);
        } else if (UB34.crossProduct(capsule->getPosition() - edge2Center).getMagnitudeSqr() < pow(capsule->getRadius(), 2)) {
            collisions = parallelCapsuleAndTileEdgeCollisions(tile->getEnd2(), tile->getEnd4(), capsule->getPosition(), capsule->getAxisDirection(), capsule->getLength());
            collision1 = get<0>(collisions);
            collision2 = get<1>(collisions);
        }
    } else {
        Vector UC34 = UB34.crossProduct(UA).normalize();
        tuple<float, float, float> edge1Solution = closestPointBetweenNonParallelLines(capsule->getPosition(), UA, edge1Center, UB34, UC34);
        tuple<float, float, float> edge2Solution = closestPointBetweenNonParallelLines(capsule->getPosition(), UA, edge2Center, UB34, UC34);
        collision1 = calculateCylinderLineCollision(capsule, edge1Center, UB34, tile->getAxis2Length(), edge1Solution);
        collision2 = calculateCylinderLineCollision(capsule, edge2Center, UB34, tile->getAxis2Length(), edge2Solution);
    }

    if (capsule->getAxisDirection().parallel(UB12)) {
        // The capsule is parallel to two of the plane's edges
        tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*> collisions;
        if (UB12.crossProduct(capsule->getPosition() - edge3Center).getMagnitudeSqr() < pow(capsule->getRadius(), 2)) {
            collisions = parallelCapsuleAndTileEdgeCollisions(tile->getEnd1(), tile->getEnd2(), capsule->getPosition(), capsule->getAxisDirection(), capsule->getLength());
            collision3 = get<0>(collisions);
            collision4 = get<1>(collisions);
        } else if (UB12.crossProduct(capsule->getPosition() - edge4Center).getMagnitudeSqr() < pow(capsule->getRadius(), 2)) {
            collisions = parallelCapsuleAndTileEdgeCollisions(tile->getEnd3(), tile->getEnd4(), capsule->getPosition(), capsule->getAxisDirection(), capsule->getLength());
            collision3 = get<0>(collisions);
            collision4 = get<1>(collisions);
        }
    } else {
        Vector UC12 = UB12.crossProduct(UA).normalize();
        tuple<float, float, float> edge3Solution = closestPointBetweenNonParallelLines(capsule->getPosition(), UA, edge3Center, UB12, UC12);
        tuple<float, float, float> edge4Solution = closestPointBetweenNonParallelLines(capsule->getPosition(), UA, edge4Center, UB12, UC12);
        collision3 = calculateCylinderLineCollision(capsule, edge3Center, UB12, tile->getAxis1Length(), edge3Solution);
        collision4 = calculateCylinderLineCollision(capsule, edge4Center, UB12, tile->getAxis1Length(), edge4Solution);
    }

    if (!(collision1 || collision2 || collision3 || collision4)) return positiveCollision ? positiveCollision : negativeCollision;

    Vector tilePoint = average({
        collision1 ? &get<0>(*collision1) : NULL,
        collision2 ? &get<0>(*collision2) : NULL,
        collision3 ? &get<0>(*collision3) : NULL,
        collision4 ? &get<0>(*collision4) : NULL
    });
    Vector capsulePoint = average({
        collision1 ? &get<1>(*collision1) : NULL,
        collision2 ? &get<1>(*collision2) : NULL,
        collision3 ? &get<1>(*collision3) : NULL,
        collision4 ? &get<1>(*collision4) : NULL
    });
    float penetrationDepth = capsule->getRadius() - (capsulePoint - tilePoint).getMagnitude();

    if (collision1) delete collision1;
    if (collision2) delete collision2;
    if (collision3) delete collision3;
    if (collision4) delete collision4;

    return new Collision(tilePoint, (capsulePoint - tilePoint).normalize(), penetrationDepth);
}

Collision* NarrowPhaseAlgorithm::ballBall(Ball* ball1, Ball* ball2) {
    return ballBall(ball1->getPosition(), ball1->getRadius(), ball2->getPosition(), ball2->getRadius());
}

Collision* NarrowPhaseAlgorithm::ballCapsule(Vector ballPos, float ballRadius, Vector capsulePos, float capsuleRadius, float capsuleLength, Vector capsuleAxisDirection, Vector capsulePositiveEnd, Vector capsuleNegativeEnd) {
    Collision* cylinderCollision = ballCylinder(ballPos, ballRadius, capsulePos, capsuleRadius, capsuleLength, capsuleAxisDirection);
    if (cylinderCollision) return cylinderCollision;
    Collision* positiveEndCollision = ballBall(ballPos, ballRadius, capsulePositiveEnd, capsuleRadius);
    if (positiveEndCollision) return positiveEndCollision;
    return ballBall(ballPos, ballRadius, capsuleNegativeEnd, capsuleRadius);
}

Collision* NarrowPhaseAlgorithm::ballCapsule(Ball* ball, Capsule* capsule) {
    return ballCapsule(ball->getPosition(), ball->getRadius(), capsule->getPosition(), capsule->getRadius(), capsule->getLength(), capsule->getAxisDirection(), capsule->getCylinderPositiveEnd(), capsule->getCylinderNegativeEnd());
}

Collision* NarrowPhaseAlgorithm::capsuleCapsule(Capsule* capsule1, Capsule* capsule2) {
    Vector UA = capsule1->getAxisDirection();
    Vector UB = capsule2->getAxisDirection();

    // Check if capsules are parallel
    if (UA.parallel(UB)) return parallelCapsules(capsule1, capsule2);

    Vector UC = UB.crossProduct(UA).normalize();
    tuple<float, float, float> solution = closestPointBetweenNonParallelLines(capsule1->getPosition(), UA, capsule2->getPosition(), UB, UC);
    
    float distanceInAxis1 = get<0>(solution);
    float distanceInAxis2 = get<1>(solution);
    Vector axis1Point = UA * distanceInAxis1 + capsule1->getPosition();
    Vector axis2Point = UB * distanceInAxis2 + capsule2->getPosition();
    float distance = abs(get<2>(solution));
    float radiusSum = capsule1->getRadius() + capsule2->getRadius();

    if (distance < radiusSum) {
        // Check collision between cylinders
        if (abs(distanceInAxis1) < capsule1->getLength() / 2 && abs(distanceInAxis2) < capsule2->getLength() / 2) {
            Vector normalVector = axis2Point - axis1Point;
            if (normalVector.isZero()) normalVector = UC;
            float radiusRatio = capsule2->getRadius() / radiusSum;
            Vector collisionPoint = axis1Point + normalVector * radiusRatio;
            return new Collision(collisionPoint, normalVector.normalize(), radiusSum - distance);
        }

        // Check collision between capsule 1 and one of the ends of capsule 2
        Vector test1BallCenter = (distanceInAxis2 > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        Collision* collision = ballCylinder(test1BallCenter, capsule2->getRadius(), capsule1->getPosition(), capsule1->getRadius(), capsule1->getLength(), capsule1->getAxisDirection());
        if (collision) {
            collision->invertNormal();
            return collision;
        }
        
        // Check collision between capsule 2 and one of the ends of capsule 1
        Vector test2BallCenter = (distanceInAxis1 > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        collision =  ballCylinder(test2BallCenter, capsule1->getRadius(), capsule2->getPosition(), capsule2->getRadius(), capsule2->getLength(), capsule2->getAxisDirection());
        if (collision) return collision;

        // Check collision between ends of the capsules
        Vector ball1Center = (distanceInAxis1 > 0) ? capsule1->getCylinderPositiveEnd() : capsule1->getCylinderNegativeEnd();
        Vector ball2Center = (distanceInAxis2 > 0) ? capsule2->getCylinderPositiveEnd() : capsule2->getCylinderNegativeEnd();
        return ballBall(ball1Center, capsule1->getRadius(), ball2Center, capsule2->getRadius());
    }

    return NULL;
}

pair<map<string, Collision>, NPCDData> NarrowPhaseAlgorithm::getCollisions(map<string, pair<Object*, Object*>>* possibleCollisions) {
    map<string, Collision> collisions;

    chrono::system_clock::time_point start;
    int capsuleCapsuleTests = 0;
    int ballCapsuleTests = 0;
    int ballBallTests = 0;
    int capsuleTileTests = 0;
    int ballTileTests = 0;
    float capsuleCapsuleTime = 0;
    float ballCapsuleTime = 0;
    float ballBallTime = 0;
    float capsuleTileTime = 0;
    float ballTileTime = 0;

    for (auto it = possibleCollisions->begin(); it != possibleCollisions->end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;

        Collision* collision = NULL;

        Ball* ball1 = dynamic_cast<Ball*>(object1);
        Ball* ball2 = dynamic_cast<Ball*>(object2);
        Capsule* capsule1 = dynamic_cast<Capsule*>(object1);
        Capsule* capsule2 = dynamic_cast<Capsule*>(object2);
        Tile* tile1 = dynamic_cast<Tile*>(object1);
        Tile* tile2 = dynamic_cast<Tile*>(object2);

        if (ball1) {
            if (ball2) {
                ballBallTests++;
                start = chrono::system_clock::now();
                collision = ballBall(ball1, ball2);
                ballBallTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
            }
            else if (capsule2) {
                ballCapsuleTests++;
                start = chrono::system_clock::now();
                collision = ballCapsule(ball1, capsule2);
                ballCapsuleTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
            }
            else if (tile2) {
                ballTileTests++;
                start = chrono::system_clock::now();
                collision = ballTile(ball1, tile2);
                ballTileTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
                if (collision) collision->invertNormal();
            }
        }
        else if (capsule1) {
            if (ball2) {
                ballCapsuleTests++;
                start = chrono::system_clock::now();
                collision = ballCapsule(ball2, capsule1);
                ballCapsuleTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
                if (collision) collision->invertNormal();
            }
            else if (capsule2) {
                capsuleCapsuleTests++;
                start = chrono::system_clock::now();
                collision = capsuleCapsule(capsule1, capsule2);
                capsuleCapsuleTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
            }
            else if (tile2) {
                capsuleTileTests++;
                start = chrono::system_clock::now();
                collision = capsuleTile(capsule1, tile2);
                capsuleTileTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
                if (collision) collision->invertNormal();
            }
        }
        else if (tile1) {
            if (ball2) {
                ballTileTests++;
                start = chrono::system_clock::now();
                collision = ballTile(ball2, tile1);
                ballTileTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
            }
            else if (capsule2) {
                capsuleTileTests++;
                start = chrono::system_clock::now();
                collision = capsuleTile(capsule2, tile1);
                capsuleTileTime += (float)chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0;
            }
            else if (tile2) {
                collision = NULL;
            }
        }

        if (collision) {
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(object1, object2);
            
            if (collisions.find(objectPair.first) == collisions.end()) {
                collision->setObjects(objectPair.second);
                auto lastFrameCollision = lastFrameCollisions.find(objectPair.first);
                if (lastFrameCollision != lastFrameCollisions.end()) {
                    collision->setSecondToLastPenetrationDepth(lastFrameCollision->second.getLastPenetrationDepth());
                    collision->setLastPenetrationDepth(lastFrameCollision->second.getPenetrationDepth());
                }

                collisions.insert(pair<string, Collision>(objectPair.first, *collision));
            }

            delete collision;
        }
    }

    lastFrameCollisions.clear();
    lastFrameCollisions = collisions;
    NPCDData npcdData = NPCDData(capsuleCapsuleTests, ballCapsuleTests, ballBallTests, capsuleTileTests, ballTileTests, capsuleCapsuleTime, ballCapsuleTime, ballBallTime, capsuleTileTime, ballTileTime);
    return pair<map<string, Collision>, NPCDData>(collisions, npcdData);
}
