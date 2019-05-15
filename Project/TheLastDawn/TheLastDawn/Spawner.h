#ifndef SpawnerTH
#define SpawnerTH

#include <vector>
#include <algorithm>

#include "Triangle.h"
#include "Rect.h"
#include "Timer.h"
#include "Random.h"
#include "Mesh.h"

using namespace SpawnArea;

template<class T>
class Spawner
{
	public:
		Spawner(unsigned number, float spread, float rate, Mesh* objectMesh, Mesh* pickUpMesh, const vector<ID3D11ShaderResourceView*>& objectTextures);
		~Spawner();
		void SetNumber(unsigned number);
		void SetSpread(float spread);
		void SetRate(float rate);
		void Start();
		void Update(float dTime, float halfScreenMultiplier, const Vector2& center, const Vector2& apex, const Vector2& direction);
		Triangle& GetSpawnTriangle();
		Rect& GetSpawnRect();
		vector<T*> GetSpawnedObjects() const;
	private:
		void UpdateObjects(float dTime);
		void PoolObjects(float halfScreenMultiplier, const Vector2& center);
		void Spawn(bool onScreen = false);
		void ReleaseObjects(vector<T*>& objects);
		void Reset();
		unsigned number_;
		float spread_;
		vector<T*> spawnedObjects_;
		vector<T*> pooledObjects_;
		Triangle spawnTriangle_;
		Rect spawnRect_;
		Rect deathLineCalculator_;
		Timer timer_;
		Mesh* pObjectMesh_;
		Mesh* pPickUpMesh_;
		vector<ID3D11ShaderResourceView*> objectTextures_;
};

template<class T>
Spawner<T>::Spawner(unsigned number, float spread, float rate, Mesh* objectMesh, Mesh* pickUpMesh, const vector<ID3D11ShaderResourceView*>& objectTextures)
	:number_(number), spread_(spread), spawnedObjects_(), pooledObjects_(),
	spawnTriangle_(), spawnRect_(), deathLineCalculator_(), timer_(rate), pObjectMesh_(objectMesh), pPickUpMesh_(pickUpMesh), objectTextures_(objectTextures)
{
	spawnedObjects_.reserve(64);
	pooledObjects_.reserve(64);
}

template<class T>
Spawner<T>::~Spawner()
{
	ReleaseObjects(spawnedObjects_);
	ReleaseObjects(pooledObjects_);
}

template<class T>
void Spawner<T>::SetNumber(unsigned number)
{
	number_ = number;
}

template<class T>
void Spawner<T>::SetSpread(float spread)
{
	spread_ = spread;
}

template<class T>
void Spawner<T>::SetRate(float rate)
{
	timer_.Reset(rate);
}

template<class T>
void Spawner<T>::Start()
{
	Reset();
	Spawn(true);
}

template<class T>
void Spawner<T>::Update(float dTime, float halfScreenMultiplier, const Vector2& center, const Vector2& apex, const Vector2& direction)
{
	PoolObjects(halfScreenMultiplier, center);
	UpdateObjects(dTime);
	if (!timer_.HasStopped())
	{
		timer_.Tick(dTime);
		return;
	}
	timer_.Reset();
	spawnTriangle_.Update(apex, direction);
	spawnRect_.Update(center);
	Spawn();
}

template<class T>
Triangle& Spawner<T>::GetSpawnTriangle()
{
	return spawnTriangle_;
}

template<class T>
Rect& Spawner<T>::GetSpawnRect()
{
  return spawnRect_;
}

template<class T>
vector<T*> Spawner<T>::GetSpawnedObjects() const
{
  return spawnedObjects_;
}

template<class T>
void Spawner<T>::UpdateObjects(float dTime)
{
	for (T* object : spawnedObjects_)
	{
		if (object->GetPosition().x < deathLineCalculator_.GetMin().x && !object->IsDisintegrating())
			object->Disintegrate();
		object->Update(dTime);
	}
}

template<class T>
void Spawner<T>::PoolObjects(float halfScreenMultiplier, const Vector2& center)
{
	deathLineCalculator_.SetMultiplier(halfScreenMultiplier);
	deathLineCalculator_.Update(center);
	auto moveCondition = [](T* object) { return object->IsDisintegrated(); };
	copy_if(spawnedObjects_.begin(), spawnedObjects_.end(),
		back_inserter(pooledObjects_), moveCondition);
	spawnedObjects_.erase(remove_if(spawnedObjects_.begin(),
		spawnedObjects_.end(), moveCondition), spawnedObjects_.end());
}

template<class T>
void Spawner<T>::Spawn(bool onScreen)
{
	for (size_t i = 0; i < number_; ++i)
	{
		int failCounter = 0;
		Vector2 nextPosition;
		do
		{
			if (failCounter < 20)
				++failCounter;
			else
				return;
			nextPosition = Random::Instance()->Range(spawnTriangle_.GetMin(), spawnTriangle_.GetMax());
		} while (any_of(spawnedObjects_.begin(), spawnedObjects_.end(),
			[=](T * spawnedObject)
			{
				return Vector2::DistanceSquared(static_cast<Vector2>(spawnedObject->GetPosition()), nextPosition) <= spread_ * spread_;
			}) || !spawnTriangle_.Contains(nextPosition) || 
				(spawnRect_.Contains(nextPosition) && !onScreen) ||
				(!spawnRect_.Contains(nextPosition) && onScreen));
		if (!pooledObjects_.empty())
		{
			// the object (in this case the planet) will spawn this pick up
			spawnedObjects_.push_back(pooledObjects_.back());
			pooledObjects_.pop_back();
		}
		else
			// the object (in this case the planet) will spawn this pick up
			spawnedObjects_.emplace_back(new T(*pObjectMesh_, 1.0f, Random::Instance()->Range(3.0f, 5.0f), pPickUpMesh_));
		spawnedObjects_.back()->SetPosition(static_cast<Vector3>(nextPosition));
		spawnedObjects_.back()->Reset(spread_ / 2);
		MaterialExt mat = spawnedObjects_.back()->GetMesh().GetSubMesh(0).material_;
		//the first texture is the noise map
		mat.pTextureRV = objectTextures_.at(Random::Instance()->Range(1, objectTextures_.size() - 1));
		mat.pNoiseMapRV = objectTextures_.at(0);
		spawnedObjects_.back()->SetOverrideMat(&mat);
	}
}

template<class T>
void Spawner<T>::ReleaseObjects(vector<T*> & objects)
{
  for (T* object : objects)
    delete object;
  objects.clear();
}

template<class T>
void Spawner<T>::Reset()
{
	deathLineCalculator_ = spawnRect_;
	copy(spawnedObjects_.begin(), spawnedObjects_.end(), back_inserter(pooledObjects_));
	spawnedObjects_.clear();
}
#endif 