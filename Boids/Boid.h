#pragma once
#include "DrawableGameObject.h"

class Boid :
	public DrawableGameObject
{
public:
	Boid();
	~Boid();

	XMFLOAT3*							getDirection() { return &m_direction; }
	void								checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj);
	void								update(float t,Predator* predator, vecBoid* drawList);

protected:
	void								setDirection(XMFLOAT3 direction);

	vecBoid								nearbyBoids(vecBoid* boidList);
	XMFLOAT3							calculateSeparationVector(vecBoid* drawList);
	XMFLOAT3							calculateAlignmentVector(vecBoid* drawList);
	XMFLOAT3							calculateCohesionVector(vecBoid* drawList);
	void								createRandomDirection();
	void								flee(Predator* predator);

	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							distanceFloat3(const XMFLOAT3& f1);
	double								distanceDouble(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);


	XMFLOAT3							m_direction;
	float								m_speed;
	//unsigned int*						m_nearbyDrawables;
};

