#pragma once

#include <Core/Object.hpp>
#include <Container/Array.hpp>
#include <Math/Transform.hpp>
#include <Misc/HashString.hpp>

namespace eagle
{
	class WorldObject : public Object
	{
	public:

		WorldObject() = default;
		~WorldObject() = default;

	protected:

		virtual void start() = 0;
		virtual void update(double) = 0;

	public:

		virtual void _internalConstruct()override
		{
			if (awake())
			{
				start();
				return;
			}
			ensure(false, "Failed to execute the awake function");
		}

		virtual void _internalUpdate([[maybe_unused]] double inDeltaTime)
		{
			if (mActive && mUpdateEnabled)
			{
				const double timeDilation = getTimeDilation();
				update(inDeltaTime * timeDilation);
			}
		}

		virtual void _internalAttachToLevel(LevelBase* inLevel);
		virtual void _internalAttachToOwner(Actor* inOwner);

	public:

		ObjectRef<LevelBase> getLevel()const noexcept
		{
			return mLevel;
		}

		ObjectRef<Actor> getOwner()const noexcept
		{
			return mOwner;
		}

		bool isOwner(const Actor* const inOwner)const noexcept;

		void addTag(const String& newTag);
		void addTags(const Array<String>& newTags);
		void removeTag(const String& inTag);
		Array<String> getTags()const noexcept;
		bool hasTag(const String& inTag)const;

		void setLocalPosition(Vec3 newLocalPos)noexcept
		{
			mTransform.setPosition(newLocalPos);
		}

		void setWorldPosition(Vec3 newWorldPos)noexcept
		{
			mTransform.setPosition(Transform::GetLocalPosition(this));
		}

		void setTransform(const Transform& newTransform)noexcept
		{
			mTransform = newTransform;
		}

		const Transform& getTransform()const noexcept
		{
			return mTransform;
		}

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

		ObjectRef<LevelBase> mLevel = nullptr;

		ObjectRef<Actor> mOwner = nullptr;

		Array<HashString> mTags = {};

		Transform mTransform = Transform::Identity();

		double mCustomTimeDilation = 1.0;

		static double sGlobalTimeDilation;

		bool mPreferGlobalTimeDilation = true;

		bool mActive = true;

		bool mPendingKill = false;

		bool mUpdateEnabled = true;

	};

	constexpr auto s = sizeof(WorldObject);
}
