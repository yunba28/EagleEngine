#include <Siv3D.hpp> // OpenSiv3D v0.6.8

#include <Core/GameManager.hpp>
#include <Core/Actor.hpp>
#include <Core/Component.hpp>

#include <Core/ExecutionOrder.hpp>

using namespace EagleEngine;

class PrintComponent :public Component
{
	bool awake()override
	{
		Print << U"do awake";
		return true;
	}

	void start()override
	{
		Print << U"do start";
	}

	void update(double delta)override
	{
		(void)delta;
		if (doOnce)
		{
			Print << U"do update";
			doOnce = false;
		}
	}

	bool dispose()override
	{
		Print << U"do dispose";
		return true;
	}

	bool doOnce = true;
};

class MyScene : public GameManager<String, void>::SceneBase
{
public:

	MyScene(const InitData& ini)
		: SceneBase(ini)
	{
		Print << U"SceneName:{}"_fmt(getSceneObject()->getName().getSequentialName());

		actor = getSceneObject()->createActor(U"MyActor");
		actor->attachComponent<PrintComponent>();
	}

	void update()override
	{
		SceneBase::update();

		if (MouseR.down())
		{
			if (auto comp = actor->getComponent<PrintComponent>();comp)
			{
				Print << U"get component";
			}
			else
			{
				Print << U"not found component";
			}
		}

		if (MouseM.down())
		{
			actor->detachComponent<PrintComponent>();
		}
	}

	ObjectPtr<Actor> actor;

	bool onceFlag = true;
};

void Main()
{
	GameManager<String> manager;
	{
		manager.add<MyScene>(U"MyScene");
	}

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
