#pragma once

#include <Core/Object.hpp>
#include <Container/Array.hpp>
#include <Interface/IUpdatable.hpp>
#include <Math/Transform.hpp>
#include <Misc/HashString.hpp>

namespace eagle
{
	class WorldObject : public Object, public IUpdatable
	{
	public:

		WorldObject() = default;
		~WorldObject() = default;

	public:

		virtual void _internalConstruct()override
		{
			ensure(awake(), "Failed to execute the awake function");
		}

		virtual void _internalUpdate(double inDeltaTime)override
		{
			if (mActive && mUpdateEnabled)
			{
				if (!mStarted)
				{
					start();
					mStarted = true;
				}

				const double timeDilation = getTimeDilation();
				update(inDeltaTime * timeDilation);
			}
		}

		virtual void _internalAttachToLevel(Level* newLevel);
		virtual void _internalAttachToOwner(WorldObject* newOwner);
		virtual void _internalDetachToOwner();

	public:

		void setTrasform(const Transform& newTransform)
		{
			mTransform = newTransform;
		}

		const Transform& getTransform()const noexcept
		{
			return mTransform;
		}

		ObjectRef<Level> getLevel()const noexcept
		{
			return mLevel;
		}

		ObjectRef<WorldObject> getOwner()const noexcept
		{
			return mOwner;
		}

		bool isOwner(const WorldObject* const inOwner)const noexcept
		{
			return mOwner == *inOwner;
		}

		void addTag(const String& newTag);
		void addTags(const Array<String>& newTags);
		void removeTag(const String& inTag);
		Array<String> getTags()const noexcept;
		bool hasTag(const String& inTag)const;

		void setCustomTimeDilation(double newTimeDilation)noexcept
		{
			mCustomTimeDilation = newTimeDilation;
		}

		double getCustomTimeDilation()const noexcept
		{
			return mCustomTimeDilation;
		}

		void setPreferGlobalTimeDilation(bool newFlag)noexcept
		{
			mPreferGlobalTimeDilation = newFlag;
		}

		bool isPreferGlobalTimeDilation()const noexcept
		{
			return mPreferGlobalTimeDilation;
		}

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

		void destroy()override;

	public:

		static void SetGlobalTimeDilation(double newTimeDilation)noexcept
		{
			sGlobalTimeDilation = newTimeDilation;
		}

		static double GetGlobalTimeDilation()noexcept
		{
			return sGlobalTimeDilation;
		}

	private:

		double getTimeDilation()const noexcept
		{
			if (mPreferGlobalTimeDilation)
			{
				return sGlobalTimeDilation != 1.0 ? sGlobalTimeDilation : mCustomTimeDilation;
			}
			else
			{
				return mCustomTimeDilation != 1.0 ? mCustomTimeDilation : sGlobalTimeDilation;
			}
		}

	private:

		Transform mTransform;

		ObjectRef<Level> mLevel = nullptr;

		ObjectRef<WorldObject> mOwner = nullptr;

		Array<HashString> mTags = {};

		double mCustomTimeDilation = 1.0;

		static double sGlobalTimeDilation;

		bool mPreferGlobalTimeDilation = true;

		bool mActive = true;

		bool mStarted = false;

		bool mPendingKill = false;

		bool mUpdateEnabled = true;

	};

	constexpr auto s = sizeof(WorldObject);
}
