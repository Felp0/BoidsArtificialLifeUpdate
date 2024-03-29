#include "Boid.h"
#include "Predator.h"


#define NEARBY_DISTANCE		40.0f	// how far boids can see
#define COHESION_WEIGHT     0.6f
#define	SEPARATION_WEIGHT	0.7f
#define	ALIGMENT_WEIGHT		1.5f
Boid::Boid()
{
	m_position = XMFLOAT3(0, 0, 0);
	m_direction = XMFLOAT3(0, 1, 0);
	m_speed = 5000.0f;
	setScale(1); 
	setColour(XMFLOAT4(0,0,0,0));
	createRandomDirection();

}

Boid::~Boid()
{
}

void Boid::createRandomDirection()
{
	float x = (float)(rand() % 10);
	x -= 5;
	float y = (float)(rand() % 10);
	y -= 5;
	float z = 0;
	setDirection(XMFLOAT3(x, y, z));
}

void Boid::setDirection(XMFLOAT3 direction)
{
	XMVECTOR v = XMLoadFloat3(&direction);
	v = XMVector3Normalize(v);
	XMStoreFloat3(&m_direction, v);
}

void Boid::flee(Predator* predator)
{
	

	if (distanceDouble(*predator->getPosition()) < 40.f)
	{
		m_direction = subtractFloat3(*predator->getPosition(), m_position);
		m_direction = normaliseFloat3(m_direction);
		m_direction = multiplyFloat3(m_direction, -1.0f);
	}
}


#define DIRECTION_DELTA 0.1f
void Boid::update(float t,Predator* predator, vecBoid* boidList)
{
	// create a list of nearby boids
	vecBoid nearBoids = nearbyBoids(boidList);

	// NOTE these functions should always return a normalised vector
	XMFLOAT3  vSeparation = calculateSeparationVector(&nearBoids);
	XMFLOAT3  vAlignment = calculateAlignmentVector(&nearBoids);
	XMFLOAT3  vCohesion = calculateCohesionVector(&nearBoids);

	vSeparation = multiplyFloat3(vSeparation, SEPARATION_WEIGHT);
	vAlignment = multiplyFloat3(vAlignment, ALIGMENT_WEIGHT);
	vCohesion = multiplyFloat3(vCohesion, COHESION_WEIGHT);
	

	XMFLOAT3 vTotal = addFloat3(vSeparation, vAlignment);
	vTotal = addFloat3(vTotal, vCohesion);
	//vTotal = normaliseFloat3(vTotal);

	//Convert float to vector
	/*XMVECTOR m_tempPosition = XMLoadFloat3(&m_position);
	XMVECTOR m_tempDirection = XMLoadFloat3(&m_direction);*/

	//Calculation
		m_direction = addFloat3(m_direction, vTotal);

	if (magnitudeFloat3(m_direction) > 0)
	{
		m_direction = multiplyFloat3(m_direction, t);
		m_direction = multiplyFloat3(m_direction, 500.0f);
		m_direction = normaliseFloat3(m_direction);
		flee(predator);
		m_position = addFloat3(m_position, m_direction);
	}
	else
	{
		createRandomDirection();
	}

	//Storing floats
	//XMStoreFloat3(&m_position, m_tempPosition);
	//XMStoreFloat3(&m_direction, m_tempDirection);

	DrawableGameObject::update(t);
}

XMFLOAT3 Boid::calculateSeparationVector(vecBoid* boidList)
{
	
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return nearby;

	

	// calculate average position of nearby

	float nearestDistance = 9999.0f;
	DrawableGameObject* nearest = nullptr;
	XMFLOAT3 directionNearestStored;

	for (Boid* boid : *boidList) {
		if (boid == this)
			continue;

		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *boid->getPosition();
		XMFLOAT3 posTry = XMFLOAT3(5, 5, 0);
		

		XMFLOAT3 directionNearest = subtractFloat3(mePos, itPos);
		float d = magnitudeFloat3(directionNearest);
		if (d < nearestDistance)
		{
			nearestDistance = d;
			nearest = boid;
			directionNearestStored = directionNearest;
		}
	}

	if (nearest != nullptr) {
		return normaliseFloat3(directionNearestStored);
	}

	return m_direction;
}

XMFLOAT3 Boid::calculateAlignmentVector(vecBoid* boidList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return nearby;
	
	
	for (Boid* boid : *boidList)
	{
		nearby = addFloat3(nearby, *boid->getDirection());

	}
		nearby = divideFloat3(nearby, (float)boidList->size());
		//nearby = subtractFloat3(nearby, m_direction);
		
	
	// your code here

	return normaliseFloat3(nearby); // return the normalised (average) direction of nearby drawables
}

XMFLOAT3 Boid::calculateCohesionVector(vecBoid* boidList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);

	if (boidList == nullptr)
		return nearby;

	// calculate average position of nearby
	for (Boid* boid : *boidList) {
		nearby = addFloat3(nearby, *boid->getPosition());
	}

	nearby = divideFloat3(nearby, (float)boidList->size());
	nearby = subtractFloat3(nearby, m_position);

	return normaliseFloat3(nearby); // nearby is the direction to where the other drawables are
}



// use but don't alter the methods below

XMFLOAT3 Boid::addFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x + f2.x;
	out.y = f1.y + f2.y;
	out.z = f1.z + f2.z;

	return out;
}

XMFLOAT3 Boid::subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x - f2.x;
	out.y = f1.y - f2.y;
	out.z = f1.z - f2.z;

	return out;
}

XMFLOAT3 Boid::multiplyFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x * scalar;
	out.y = f1.y * scalar;
	out.z = f1.z * scalar;

	return out;
}

XMFLOAT3 Boid::divideFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x / scalar;
	out.y = f1.y / scalar;
	out.z = f1.z / scalar;

	return out;
}

XMFLOAT3 Boid::distanceFloat3(const XMFLOAT3& f1)
{
	XMFLOAT3 out;
	out.x = sqrt(f1.x - m_position.x * f1.x - m_position.x);
	out.y = sqrt(f1.y - m_position.y * f1.y - m_position.y) ;
	out.z = sqrt(f1.z - m_position.z * f1.z - m_position.z);

	return out;
}

double Boid::distanceDouble(XMFLOAT3& f1)
{
	double xDisplacement = f1.x - m_position.x;
	double yDisplacement = f1.y - m_position.y;
	double zDisplacement = f1.z - m_position.z;

	return sqrt(xDisplacement * xDisplacement + yDisplacement * yDisplacement + zDisplacement * zDisplacement);
}

float Boid::magnitudeFloat3(XMFLOAT3& f1)
{
	return sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));
}

XMFLOAT3 Boid::normaliseFloat3(XMFLOAT3& f1)
{
	float length = sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));

	f1.x /= length;
	f1.y /= length;
	f1.z /= length;

	return f1;
}

vecBoid Boid::nearbyBoids(vecBoid* boidList)
{
	vecBoid nearBoids;
	if (boidList->size() == 0)
		return nearBoids;

	for (Boid* boid : *boidList) {
		// ignore self
		if (boid == this)
			continue;

		// get the distance between the two
		XMFLOAT3 vB = *(boid->getPosition());
		XMFLOAT3 vDiff = subtractFloat3(m_position, vB);
		float l = magnitudeFloat3(vDiff);
		if (l < NEARBY_DISTANCE) {
			nearBoids.push_back(boid);
		}
	}

	return nearBoids;
}


void Boid::checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj)
{
	// remove me

	XMFLOAT4 v4;
	v4.x = m_position.x;
	v4.y = m_position.y;
	v4.z = m_position.z;
	v4.w = 1.0f;

	XMVECTOR vScreenSpace = XMLoadFloat4(&v4);
	XMVECTOR vScreenSpace2 = XMVector4Transform(vScreenSpace, view);
	XMVECTOR vScreenSpace3 = XMVector4Transform(vScreenSpace2, proj);

	XMFLOAT4 v;
	XMStoreFloat4(&v, vScreenSpace3);
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w /= v.w;

	float fOffset = 10; // a suitable distance to rectify position within clip space
	if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
	{
		if (v.x < -1 || v.x > 1) {
			v4.x = -v4.x + (fOffset * v.x);
		}
		else if (v.y < -1 || v.y > 1) {
			v4.y = -v4.y + (fOffset * v.y);
		}

		// throw a bit of randomness into the mix
		//createRandomDirection();

		// method 1 - appear on the other side
		m_position.x = v4.x;
		m_position.y = v4.y;
		m_position.z = v4.z;

		// method2 - bounce off sides and head to centre
		/*if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
		{
			m_direction = multiplyFloat3(m_direction, -1);;
			m_direction = normaliseFloat3(m_direction);
		}*/
	}

	return;
}