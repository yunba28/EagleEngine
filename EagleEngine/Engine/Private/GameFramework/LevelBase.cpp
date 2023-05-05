#include <GameFramework/LevelBase.hpp>

namespace eagle
{
	LevelBase::LevelBase()
		: mSubSystems()
		, mWorldObjectSubSystem()
	{
		mWorldObjectSubSystem = addSubSystem<WorldObjectSubSystem>();
		ensure(mWorldObjectSubSystem != nullptr, "Failed create WorldObjectSubSystem");
	}

	LevelBase::~LevelBase()
	{
		for (const auto& system : mSubSystems)
		{
			if (IsValidByObject(system.get()))
			{
				delete system.get();
			}
		}
		mSubSystems.clear();
	}

	void LevelBase::_internalUpdate(double inDeltaTime)
	{
		for (const auto& subSystem : mSubSystems)
		{
			subSystem->update(inDeltaTime);
		}

		update(inDeltaTime);
	}

	void LevelBase::_internalDraw() const
	{

	}

	ObjectPtr<SubSystem> LevelBase::addSubSystem(const ObjectClass& inObjectClass)
	{
		if (inObjectClass.hasInherited(ObjectInherited::SubSystem))
		{
			ObjectPtr<SubSystem> newSubSystem = static_cast<SubSystem*>(inObjectClass());
			mSubSystems.push_back(newSubSystem);
			return newSubSystem;
		}
		return ObjectPtr<SubSystem>();
	}
}
