#pragma once

#include <Core/Object.hpp>
#include <Container/Array.hpp>
#include <Misc/HashString.hpp>

namespace eagle
{
	class WorldObject : public Object
	{
	public:

		WorldObject() = default;
		~WorldObject() = default;

	protected:

		virtual bool awake() = 0;
		virtual void start() = 0;
		virtual void update(double) = 0;
		virtual bool dispose() = 0;

	public:

		virtual void _internalConstruct()
		{
			if (awake())
			{
				start();
				return;
			}
			assert(("Failed to execute the awake function", false));
		}

		virtual void _internalDestruct()
		{
#if _DEBUG
			assert(("Failed to execute the dispose function",dispose()));
#else
			dispose();
#endif
		}

		virtual void _internalUpdate([[maybe_unused]] double inDeltaTime)
		{
			if (mActive && mUpdateEnabled)
			{
				update(inDeltaTime);
			}
		}

		virtual void _internalAttachToLevel(LevelBase* inLevel);
		virtual void _internalAttachToOwner(Actor* inOwner);

	public:

		ObjectPtr<Level> level()const noexcept
		{
			return mLevel;
		}

		ObjectPtr<Actor> owner()const noexcept
		{
			return mOwner;
		}

		void addTag(const String& newTag);
		void addTags(const Array<String>& newTags);
		void removeTag(const String& inTag);
		Array<String> getTags()const noexcept;
		bool hasTag(const String& inTag)const;

		void setActive(bool newActive)noexcept
		{
			if (!mPendingKill)
			{
				mActive = newActive;
			}
		}

		bool isActive()const noexcept
		{
			return mActive;
		}

		bool pendingKill()const noexcept
		{
			return mPendingKill;
		}

		void setUpdateEnable(bool newEnabled)noexcept
		{
			mUpdateEnabled = newEnabled;
		}

		bool isUpdateEnabled()const noexcept
		{
			return mUpdateEnabled;
		}

		void destroy();

	private:

		ObjectPtr<Level> mLevel;

		ObjectPtr<Actor> mOwner;

		Array<HashString> mTags;

		bool mActive;

		bool mPendingKill;

		bool mUpdateEnabled;

	};
}
