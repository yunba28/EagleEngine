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

class MyLevel : public Level
{
public:

	void awake();

};

void Main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ObjectPtr<World> world = CreateObjectClass<World>()();
	{
		world->registerLevel<MyLevel>(U"MyLevel");
	}

	while (System::Update())
	{
		ClearPrint();
		if (!world->update())
			break;
	}

	_CrtDumpMemoryLeaks();
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

void MyLevel::awake()
{
	createActor<MyActor>(U"MyActor");
}
