#include "Random.h"

Random::Random()
	: mEngine(steady_clock::now().time_since_epoch().count())
{
	
}

Random * Random::Instance()
{
  static Random srng;
  return &srng;
}

Random::~Random()
{

}

int Random::Range(int min, int max)
{
	uniform_int_distribution<int> distribution(min, max);
	return distribution(mEngine);
}

float Random::Range(float min, float max)
{
	//nextAfter gives the next float number from max, towards max + 1
	uniform_real_distribution<float> distribution(min, nextafter(max, max + 1));
	return distribution(mEngine);
}

Vector2 Random::Range(const Vector2& min, const Vector2& max)
{
	return Vector2(Range(min.x, max.x), Range(min.y, max.y));
}

Vector3 Random::Range(const Vector3& min, const Vector3& max)
{
	return Vector3(Range(min.x, max.x), Range(min.y, max.y), Range(min.z, max.z));
}
