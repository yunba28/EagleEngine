#include <GameFramework/Level.hpp>

#include <GameFramework/World.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Core/RendererSubSystem.hpp>

namespace eagle
{
	Level::Level()
		: mWorld()
		, mSubSystems()
		, mWorldObjectSubSystem()
		, mRendererSubSystem()
		, mStarted(false)
	{
		mWorldObjectSubSystem = addSubSystem<WorldObjectSubSystem>();
		ensure(mWorldObjectSubSystem != nullptr, "Failed create WorldObjectSubSystem");
		mRendererSubSystem = addSubSystem<RendererSubSystem>();
		ensure(mRendererSubSystem != nullptr, "Failed create RendererSubSystem");
	}

	void Level::_internalAttachToWorld(World* newWorld)
	{
		ensure(mWorld.invalid(), "The level is already attached in World");
		mWorld = newWorld;
	}

	void Level::_internalUpdate(double inDeltaTime)
	{
		if (!mStarted)
		{
			start();
			mStarted = true;
		}

		for (const auto& subSystem : mSubSystems)
		{
			subSystem->_internalUpdate(inDeltaTime);
		}

		update(inDeltaTime);
	}

	void Level::_internalDraw() const
	{
		mRendererSubSystem->draw();
	}

	ObjectRef<SubSystem> Level::addSubSystem(const ObjectClass& inObjectClass)
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

	ObjectRef<WorldObject> Level::createActor(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (!inObjectClass.hasInherited(ObjectInherited::Actor))
			return ObjectRef<WorldObject>();
		return mWorldObjectSubSystem->createObject(inObjectClass, newName, newOwner);
	}

	ObjectRef<WorldObject> Level::createComponent(const ObjectClass& inObjectClass, const String& newName, Actor* newOwner)
	{
		if (!inObjectClass.hasInherited(ObjectInherited::Component))
			return ObjectRef<WorldObject>();
		return mWorldObjectSubSystem->createObject(inObjectClass, newName, newOwner);
	}

	ObjectRef<WorldObject> Level::findByName(const TypeIndex& inFindType, const String& inName) const
	{
		return mWorldObjectSubSystem->findByName(inFindType, inName);
	}

	ObjectRef<WorldObject> Level::findByTag(const TypeIndex& inFindType, const String& inTag) const
	{
		return mWorldObjectSubSystem->findByTag(inFindType, inTag);
	}

	Array<ObjectRef<WorldObject>> Level::findsByTag(const String& inTag) const
	{
		return mWorldObjectSubSystem->findsByTag(inTag);
	}

	Array<ObjectRef<WorldObject>> Level::findsByTag(const TypeIndex& inFindType, const String& inTag) const
	{
		return mWorldObjectSubSystem->findsByTag(inFindType, inTag);
	}

	ObjectRef<WorldObject> Level::findByOwner(const TypeIndex& inFindType, const Actor* const inOwner) const
	{
		return mWorldObjectSubSystem->findByOwner(inFindType, inOwner);
	}

	Array<ObjectRef<WorldObject>> Level::findsByOwner(const Actor* const inOwner) const
	{
		return mWorldObjectSubSystem->findsByOwner(inOwner);
	}

	ObjectRef<SubSystem> Level::findSubSystem(const TypeIndex& inFindType) const
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
