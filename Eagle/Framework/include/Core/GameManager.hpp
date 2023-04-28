#pragma once

#include <Core/Internal/SceneObject.hpp>
#include <String/String.hpp>

#include <Siv3D/SceneManager.hpp>

namespace EagleEngine
{
	template<class State, class SharedData = void>
	class GameManager;

	template<class State, class SharedData>
	class SceneBase : public s3d::IScene<State, SharedData>
	{
	public:

		explicit SceneBase(const IScene<State, SharedData>::InitData& _initData)
			: IScene<State, SharedData>::IScene(_initData)
			, mSceneObject(new SceneObject())
		{
			if constexpr (std::is_same_v<decltype(_initData.state),String>)
			{
				mSceneObject->setName(_initData.state);
			}
		}

		virtual void update()override
		{
			mSceneObject->update(s3d::Scene::DeltaTime());
		}

		virtual void updateFadeIn(double _progress)override
		{
			mSceneObject->updateFadeIn(s3d::Scene::DeltaTime(), _progress);
		}

		virtual void updateFadeOut(double _progress)override
		{
			mSceneObject->updateFadeOut(s3d::Scene::DeltaTime(), _progress);
		}

	public:

		ObjectPtr<SceneObject> getSceneObject()const noexcept { return mSceneObject; }

	private:

		ObjectPtr<SceneObject> mSceneObject;

	};

	template<class State, class SharedData>
	class GameManager : protected s3d::SceneManager<State, SharedData>
	{
	public:

		using SceneBase = SceneBase<State, SharedData>;

		using SceneManager<State, SharedData>::SceneManager;
		using SceneManager<State, SharedData>::add;
		using SceneManager<State, SharedData>::init;
		using SceneManager<State, SharedData>::changeScene;
		using SceneManager<State, SharedData>::update;
		using SceneManager<State, SharedData>::get;
		using SceneManager<State, SharedData>::setFadeColor;
		using SceneManager<State, SharedData>::getFadeColor;
		using SceneManager<State, SharedData>::notifyError;
	};
}
