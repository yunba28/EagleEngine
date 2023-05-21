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
			if (bCanCreateTransform)
			{
				mTransform = std::make_unique<Transform>(Transform::Identity());
			}

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

		void setLocalTrasform(const Transform& newTransform)
		{
			if (mTransform)
			{
				*mTransform = newTransform;
			}
		}

		Transform getLocalTransform()const
		{
			return mTransform ? *mTransform : Transform::Identity();
		}

		void setWorldTransform(const Transform& newTransform)
		{
			if (mTransform)
			{
				mTransform->setPosition(Transform::WorldToLocalPosition(this,newTransform.getPosition()));
				mTransform->setRotation(Transform::WorldToLocalRotation(this, newTransform.getRotation()));
				mTransform->setScale(Transform::WorldToLocalScale(this, newTransform.getScale()));
			}
		}

		Transform getWorldTransform()const
		{
			if (mTransform)
			{
				return Transform
				{
					getWorldPosition(),
					getWorldRotation(),
					getWorldScale()
				};
			}
			return Transform::Identity();
		}

		void setLocalPosition(const Vec3& newLocalPos)noexcept
		{
			if (mTransform)
			{
				mTransform->setPosition(newLocalPos);
			}
		}

		Vec3 getLocalPosition()const noexcept
		{
			return mTransform ? mTransform->getPosition() : Vec3{};
		}

		void setWorldPosition(const Vec3& newWorldPos)noexcept
		{
			if (mTransform)
			{
				mTransform->setPosition(Transform::WorldToLocalPosition(this, newWorldPos));
			}
		}

		Vec3 getWorldPosition()const
		{
			return Transform::LocalToWorldPosition(this, mTransform ? mTransform->getPosition() : Vec3{});
		}

		void setLocalRotation(const Quaternion& newLocalRot)noexcept
		{
			if (mTransform)
			{
				mTransform->setRotation(newLocalRot);
			}
		}

		void setLocalRotation(const Vec3& newEuler)noexcept
		{
			if (mTransform)
			{
				mTransform->setRotation(newEuler);
			}
		}

		Quaternion getLocalRotation()const noexcept
		{
			return mTransform ? mTransform->getRotation() : Quaternion::Identity();
		}

		void setWorldRotation(const Quaternion& newWorldRot)noexcept
		{
			if (mTransform)
			{
				mTransform->setRotation(Transform::WorldToLocalRotation(this, newWorldRot));
			}
		}

		void setWorldRotation(const Vec3& newEuler)noexcept
		{
			if (mTransform)
			{
				auto rot = Quaternion::RollPitchYaw(newEuler.x, newEuler.y, newEuler.z);
				mTransform->setRotation(Transform::WorldToLocalRotation(this, rot));
			}
		}

		Quaternion getWorldRotation()const
		{
			return Transform::LocalToWorldRotation(this, mTransform ? mTransform->getRotation() : Quaternion::Identity());
		}

		void setLocalScale(const Vec3& newScale)noexcept
		{
			if (mTransform)
			{
				mTransform->setScale(newScale);
			}
		}

		Vec3 getLocalScale()const noexcept
		{
			return mTransform ? mTransform->getScale() : Vec3{ 1,1,1 };
		}

		void setWorldScale(const Vec3& newScale)
		{
			if (mTransform)
			{
				mTransform->setScale(Transform::LocalToWorldScale(this, newScale));
			}
		}

		Vec3 getWorldScale()const
		{
			return Transform::LocalToWorldScale(this, mTransform ? mTransform->getScale() : Vec3{ 1,1,1 });
		}

		void addLocalPosition(const Vec3& inVec)
		{
			mTransform->addPosition(inVec);
		}

		void addWorldPosition(const Vec3& inVec)
		{
			getRoot()->addLocalPosition(inVec);
		}

		void addLocalRotation(const Quaternion& inQuat)
		{
			mTransform->addRotation(inQuat);
		}

		void addLocalRotation(const Vec3& inEuler)
		{
			mTransform->addRotation(inEuler);
		}

		void addWorldRotation(const Quaternion& inQuat)
		{
			getRoot()->addLocalRotation(inQuat);
		}

		void addWorldRotation(const Vec3& inEuler)
		{
			getRoot()->addLocalRotation(inEuler);
		}

		Vec3 getLocalForward()const
		{
			return Vec3::Forward() * getLocalRotation();
		}

		Vec3 getLocalRight()const
		{
			return Vec3::Right() * getLocalRotation();
		}

		Vec3 getLocalUp()const
		{
			return Vec3::Up() * getLocalRotation();
		}

		Vec3 getWorldForward()const
		{
			return Vec3::Forward() * getWorldRotation();
		}

		Vec3 getWorldRight()const
		{
			return Vec3::Right() * getWorldRotation();
		}

		Vec3 getWorldUp()const
		{
			return Vec3::Up() * getWorldRotation();
		}

	public:

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

		std::unique_ptr<Transform> mTransform;

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

	protected:

		bool bCanCreateTransform;

	};

	constexpr auto s = sizeof(WorldObject);
}
