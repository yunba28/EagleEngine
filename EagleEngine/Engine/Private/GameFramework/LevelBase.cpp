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
		mSubSystems.clear();
	}

	void LevelBase::_internalConstruct()
	{
		awake();
	}

	void LevelBase::_internalDestruct()
	{
		dispose();
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
		draw();
	}

	ObjectRef<SubSystem> LevelBase::addSubSystem(const ObjectClass& inObjectClass)
	{
		if (inObjectClass.hasInherited(ObjectInherited::SubSystem))
		{
			SubSystem* newSubSystem = static_cast<SubSystem*>(inObjectClass());
			newSubSystem->_internalAttachToLevel(this);
			mSubSystems.push_back(ObjectPtr<SubSystem>(newSubSystem));
			return ObjectRef<SubSystem>(newSubSystem);
		}
		return ObjectRef<SubSystem>();
	}
}
