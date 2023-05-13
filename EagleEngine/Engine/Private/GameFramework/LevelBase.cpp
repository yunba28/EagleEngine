#include <GameFramework/LevelBase.hpp>

#include <Core/WorldObjectSubSystem.hpp>

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

	ObjectRef<WorldObject> LevelBase::createActor(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if(!inObjectClass.hasInherited(ObjectInherited::Actor))
			return ObjectRef<WorldObject>();
		return mWorldObjectSubSystem->createObject(inObjectClass, newName, newOwner);
	}

	ObjectRef<WorldObject> LevelBase::createComponent(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (!inObjectClass.hasInherited(ObjectInherited::Component))
			return ObjectRef<WorldObject>();
		return mWorldObjectSubSystem->createObject(inObjectClass, newName, newOwner);
	}

	ObjectRef<WorldObject> LevelBase::createSubLevel(const ObjectClass& inObjectClass, const String& newName)
	{
		if (!inObjectClass.hasInherited(ObjectInherited::SubLevel))
			return ObjectRef<WorldObject>();
		return mWorldObjectSubSystem->createObject(inObjectClass, newName, nullptr);
	}

	ObjectRef<WorldObject> LevelBase::findByName(const TypeIndex& inFindType, const String& inName) const
	{
		return mWorldObjectSubSystem->findByName(inFindType, inName);
	}

	ObjectRef<WorldObject> LevelBase::findByTag(const TypeIndex& inFindType, const String& inTag) const
	{
		return mWorldObjectSubSystem->findByTag(inFindType, inTag);
	}

	Array<ObjectRef<WorldObject>> LevelBase::findsByTag(const String& inTag) const
	{
		return mWorldObjectSubSystem->findsByTag(inTag);
	}

	Array<ObjectRef<WorldObject>> LevelBase::findsByTag(const TypeIndex& inFindType, const String& inTag) const
	{
		return mWorldObjectSubSystem->findsByTag(inFindType, inTag);
	}

	ObjectRef<WorldObject> LevelBase::findByOwner(const TypeIndex& inFindType, const Actor* const inOwner) const
	{
		return mWorldObjectSubSystem->findByOwner(inFindType, inOwner);
	}

	Array<ObjectRef<WorldObject>> LevelBase::findsByOwner(const Actor* const inOwner) const
	{
		return mWorldObjectSubSystem->findsByOwner(inOwner);
	}

	ObjectRef<SubSystem> LevelBase::findSubSystem(const TypeIndex& inFindType) const
	{
		auto found = std::find_if(mSubSystems.begin(), mSubSystems.end(), [&inFindType](const ObjectPtr<SubSystem>& inSubSystem)
		{
			return inSubSystem->isA(inFindType);
		});

		if (found != mSubSystems.end())
		{
			return ObjectRef<SubSystem>{*found};
		}

		return ObjectRef<SubSystem>{nullptr};
	}
}
