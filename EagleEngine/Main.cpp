#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/World.hpp>
#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Render/Renderer.hpp>
#include <Components/Camera2DComponent.hpp>

using namespace eagle;

class MyActor : public Actor
{
	bool awake()override;
	void start()override;
	void update(double inDeltaTime)override;

	ObjectRef<Camera2DComponent> camera;
};

class MyActor2 : public Actor
{
	void update(double inDeltaTime)override;
};

class MyComponent : public Component
{
	void update(double inDeltaTime)override;
};

class MyRenderer : public Renderer
{
public:

	MyRenderer()
		: pos(s3d::Scene::CenterF())
	{
		setRenderType(RenderType::Screen);
	}

private:

	void update(double inDelta)override
	{
		const double scale = (KeyRight.pressed() - KeyLeft.pressed());
		const double speed = 300;

		pos.x += speed * scale * inDelta;

		if (KeyLShift.down())
		{
			WorldObjectSubSystem::HitStop(0.1, 2.0);
		}
	}

	void draw()const override
	{
		Circle{ pos,20 }.draw();
	}

	Vec2 pos;
};

class MyUI : public Renderer
{
public:

	MyUI()
	{
		setRenderType(RenderType::UI);
	}

private:

	virtual void draw()const override
	{
		const Rect scWindow = s3d::Scene::Rect();
		SimpleGUI::Button(U"BUTTON", scWindow.tr().moveBy(-200, 0), 200);
	}

};

class MyLevel : public Level
{
public:

	bool awake();

	void update(double inDeltaTime);

};

class MyLevel2 : public Level
{
public:

	bool awake();

	void update(double inDeltaTime);

};

void Main()
{
	ObjectPtr<World> world = CreateObjectClass<World>()();
	{
		world->registerLevel<MyLevel>(U"MyLevel")
			.registerLevel<MyLevel2>(U"MyLevel2");
	}

	while (System::Update())
	{
		ClearPrint();
		if (!world->update())
			break;
	}

	world.reset(nullptr);

	// 基本的にリークが発生することはない
	// デバッグ時のみ有効
	_DumpObjectMemryLeaks();
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

bool MyActor::awake()
{
	attachComponent<MyComponent>();
	attachComponent<MyRenderer>();
	attachComponent<MyUI>();
	camera = attachComponent<Camera2DComponent>();

	return true;
}

void MyActor::start()
{
	camera->setUpdateEnable(true);
	camera->setDrawEnable(true);
}

void MyActor::update(double inDeltaTime)
{
	auto level = getLevel();

	if (level)
	{
		Print << level->getName().toString();
		Print << level->isA(typeid(MyLevel));
	}

	Print << inDeltaTime;
}

bool MyLevel::awake()
{
	return !createActor<MyActor>(U"MyActor").invalid();
}

void MyLevel::update(double inDeltaTime)
{
	if (KeyEnter.down())
	{
		getWorld()->changeLevel(U"MyLevel2");
	}
}

void MyActor2::update(double inDeltaTime)
{
	auto level = getLevel();

	if (level)
	{
		Print << level->getName().toString();
		Print << level->isA(typeid(MyLevel2));
	}

	Print << inDeltaTime;
}

bool MyLevel2::awake()
{
	return !createActor<MyActor2>(U"MyActor2").invalid();
}

void MyLevel2::update(double inDeltaTime)
{
	if (KeyEnter.down())
	{
		getWorld()->changeLevel(U"MyLevel");
	}
}

void MyComponent::update(double inDeltaTime)
{
	Print << getName().getSequentialName();
}
