#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/World.hpp>
#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Render/Renderer.hpp>
#include <Components/Camera2DComponent.hpp>

using namespace eagle;



void Main()
{
	ObjectPtr<World> world = CreateObjectClass<World>()();
	{
		
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
