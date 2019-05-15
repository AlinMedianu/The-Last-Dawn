#ifndef RandomH
#define RandomH

#include <d3d11.h>
#include <DirectXColors.h>
#include <cmath>
#include <chrono>
#include <random>

#include "SimpleMath.h"

using namespace std;
using namespace chrono;
using namespace DirectX;
using namespace SimpleMath;

class Random
{
	public:
		static Random* Instance();
		~Random();
		//[min, max]
		int Range(int min, int max);
		//[min, max]
		float Range(float min, float max);
		//[min, max]
		Vector2 Range(const Vector2& min, const Vector2& max);
		//[min, max]
		Vector3 Range(const Vector3& min, const Vector3& max);
	private:
		Random();
		default_random_engine mEngine;
};
#endif 

