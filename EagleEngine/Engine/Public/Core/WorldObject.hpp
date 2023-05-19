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

		WorldObject();

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

		void setLocalPosition(const Vec3& newLocalPos)noexcept
		{
			mTransform.setPosition(newLocalPos);
		}

		const Vec3& getLocalPosition()const noexcept
		{
			return mTransform.getPosition();
		}

		void setWorldPosition(const Vec3& newWorldPos)noexcept
		{
			mTransform.setPosition(Transform::WorldToLocalPosition(this, newWorldPos));
		}

		Vec3 getWorldPosition()const
		{
			return Transform::LocalToWorldPosition(this, mTransform.getPosition());
		}

		void setLocalRotation(const Quaternion& newLocalRot)noexcept
		{
			mTransform.setRotation(newLocalRot);
		}

		void setLocalRotation(const Vec3& newEuler)noexcept
		{
			mTransform.setRotation(newEuler);
		}

		const Quaternion& getLocalRotation()const noexcept
		{
			return mTransform.getRotation();
		}

		void setWorldRotation(const Quaternion& newWorldRot)noexcept
		{
			mTransform.setRotation(Transform::WorldToLocalRotation(this, newWorldRot));
		}

		void setWorldRotation(const Vec3& newEuler)noexcept
		{
			auto rot = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
			mTransform.setRotation(Transform::WorldToLocalRotation(this, rot));
		}

		Quaternion getWorldRotation()const
		{
			return Transform::LocalToWorldRotation(this,mTransform.getRotation());
		}

		void addLocalPosition(const Vec3& inVec)
		{
			mTransform.addPosition(inVec);
		}

		void addWorldPosition(const Vec3& inVec)
		{
			getRoot()->addLocalPosition(inVec);
		}

		void addLocalRotation(const Quaternion& inQuat)
		{
			mTransform.addRotation(inQuat);
		}

		void addLocalRotation(const Vec3& inEuler)
		{
			mTransform.addRotation(inEuler);
		}

		void addWorldRotation(const Quaternion& inQuat)
		{
			getRoot()->addLocalRotation(inQuat);
		}

		void addWorldRotation(const Vec3& inEuler)
		{
			getRoot()->addLocalRotation(inEuler);
		}

		Vec3 getForward()const
		{
			return Vec3::Forward() * getWorldRotation();
		}

		Vec3 getRight()const
		{
			return Vec3::Right() * getWorldRotation();
		}

		Vec3 getUp()const
		{
			return Vec3::Up() * getWorldRotation();
		}

		ObjectRef<Level> getLevel()const noexcept
		{
			return mLevel;
		}

		void attachToActor(Actor* newOwner);
		void attachToComponent(Component* newOwner);

		ObjectRef<WorldObject> getOwner()const noexcept
		{
			return mOwner;
		}

		ObjectRef<WorldObject> getRoot()const;

		void detachToOwner();

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

		ObjectRef<Level> mLevel;

		ObjectRef<WorldObject> mSelf;

		ObjectRef<WorldObject> mOwner;

		Array<HashString> mTags;

		double mCustomTimeDilation;

		static double sGlobalTimeDilation;

		bool mPreferGlobalTimeDilation;

		bool mActive;

		bool mStarted;

		bool mPendingKill;

		bool mUpdateEnabled;

	};

	constexpr auto s = sizeof(WorldObject);
}
