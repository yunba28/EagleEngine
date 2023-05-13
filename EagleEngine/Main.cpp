#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/Actor.hpp>
#include <GameFramework/World.hpp>

using namespace eagle;

class MyActor : public Actor
{
	void update(double inDeltaTime)override;
};

class MyActor2 : public Actor
{
	void update(double inDeltaTime)override;
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
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ObjectPtr<World> world = CreateObjectClass<World>()();
	{
		world->registerLevel<MyLevel>(U"MyLevel")
			.registerLevel<MyLevel2>(U"MyLevel2");
	}

	WorldObjectSubSystem;

	while (System::Update())
	{
		ClearPrint();
		if (!world->update())
			break;
	}

	//_CrtDumpMemoryLeaks();
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
