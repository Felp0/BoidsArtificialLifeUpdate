#pragma once
#include "DrawableGameObject.h"
#include "Boid.h"

class Predator:
	public DrawableGameObject
{
public:
	Predator();
	~Predator();

	XMFLOAT3*							getDirection() { return &m_direction; }
	void								checkIsOnScreenAndFix(const XMMATRIX& view, const XMMATRIX& proj);
	void								update(float t,Boid* fish, vecPredator* predatorList);

protected:
	void								setDirection(XMFLOAT3 direction);

	XMFLOAT3							addFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	XMFLOAT3							multiplyFloat3(XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);
	float								magnitudeFloat3(XMFLOAT3& f1);
	void								createRandomDirection();
	void								pursuit(Boid* fish);

	XMFLOAT3							m_test;
	XMFLOAT3							m_direction;
	XMFLOAT3							m_futurePoint;
	float								m_speed;

	Boid*								m_boid;

};

