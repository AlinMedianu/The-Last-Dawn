//-------------------------------------------------------------------
// ModeManager.cpp
// Singleton class that keeps track of all game modes, allowing them
// to be updated and rendered as needed
//-------------------------------------------------------------------


#include "../CRC.h"
#include "../Error.h"
#include "../UtilsDefines.h"

// Application includes
#include "Mode.h"
#include "ModeManager.h"
#include "../Singleton.h"
#include "../WindowUtils.h"

using namespace UDEF;


void ModeManager::OnKey(char key)
{
	for (unsigned int i = 0; i < modes_.size(); i++)
	{
		if (!modes_[i]->IsActive())
			continue;
		modes_[i]->OnKey(key);
	}
}

void ModeManager::Init()
{
	for (unsigned int i = 0; i < modes_.size(); i++)
		modes_[i]->Init();
}

ModeManager::ModeManager() : pNextMode_(0), nextModeHash_(UDEF::MAX_UINT)
{
}

//-------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------
ModeManager::~ModeManager()
{
	for (unsigned int i = 0; i < modes_.size(); i++)
	{
		Mode* lpMode = modes_[i];
		delete lpMode;
	}
	modes_.clear();
}

void ModeManager::SwitchMode(const char *apModeName)
{
	pNextMode_ = GetMode(apModeName);
}

//-------------------------------------------------------------------
// Update all currently active game modes
//-------------------------------------------------------------------
void ModeManager::Update(float aTime)
{
	// Call the update method of all currently active modes
	Mode *pActive=NULL;
	for (unsigned int i = 0; i < modes_.size(); i++)
	{
		if(!modes_[i]->IsActive())
			continue;
		pActive=modes_[i];
		pActive->Update(aTime);
		if(!pNextMode_)
			break;
		MPOD_ASSERT_MSG(pActive!=pNextMode_,"Switching to current mode");
		pActive->Exit();
		pNextMode_->Enter();
		pActive=pNextMode_;
		pNextMode_=nullptr;
		break;
	}
	if(!pActive)
	{
		MPOD_ASSERT_MSG(pNextMode_,"No active mode");
		pActive=pNextMode_;
		pNextMode_=nullptr;
		pActive->Enter();
	}
}

//-------------------------------------------------------------------
// Render all currently visible game modes
//-------------------------------------------------------------------
void ModeManager::Render(float dTime)
{
	// Call the render method of all currently visible modes
	for (unsigned int i = 0; i < modes_.size(); i++)
	{
		Mode* lpMode = modes_[i];
		if (lpMode->IsVisible())
			lpMode->Render(dTime);
	}
}

//-------------------------------------------------------------------
// Add a mode instance to the mode manager.  This method is called in
// Mode base class constructor, so should never need to be called
// by the user
//-------------------------------------------------------------------
void ModeManager::AddMode(Mode* apMode)
{
	modes_.push_back(apMode);
}

//-------------------------------------------------------------------
// Gets the mode instance with the given name
//-------------------------------------------------------------------
Mode* ModeManager::GetMode(const char* apModeName)
{
	unsigned int lHash = SCRC::GenerateCRC32(apModeName, strlen(apModeName) ); //IwHashString(apModeName);

	for (unsigned int i = 0; i < modes_.size(); i++)
	{
		Mode* lpMode = modes_[i];
		if (lpMode->HasHash(lHash))
			return lpMode;
	}

	MPOD_ASSERT_MSG(false, "Unable to find a registered Mode called '%s'", apModeName);
	return nullptr;
}

Mode * ModeManager::GetMode( int hash )
{
	for(Modes::iterator it=modes_.begin(); it!=modes_.end(); ++it)
	{
		if((*it)->GetHash()==hash)
			return *it;
	}
	MPOD_ASSERT(false);
	return nullptr;
}


