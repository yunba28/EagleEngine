#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/World.hpp>
#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Render/Renderer.hpp>
#include <Components/Camera2DComponent.hpp>

using namespace eagle;

class BoxComponent : public Renderer
{
public:

	BoxComponent()
	{
		setRenderType(RenderType::WorldSpace);
	}

private:

	void draw()const override
	{
		auto [pos, rot, scl] = static_cast<TransformRaw>(getWorldTransform());
		Box{ pos,mExtent * 2}.scaled(scl).draw(rot, mColor);

		auto fwd = getWorldForward() * 5;
		Line3D{ pos,pos + fwd }.draw(Palette::Red);
	}

public:

	void setExtent(Vec3 newExtent)
	{
		mExtent = newExtent;
	}

	void setColor(ColorF newColor)
	{
		mColor = newColor;
	}

private:

	Vec3 mExtent{ 2.5,2.5,2.5 };
	ColorF mColor{ Palette::Orange };

};

class BoxActor : public Actor
{
private:

	bool awake()override
	{
		mBoxComp = attachComponent<BoxComponent>();
		return true;
	}

public:

	void setExtent(Vec3 newExtent)
	{
		mBoxComp->setExtent(newExtent);
	}

	void setColor(ColorF newColor)
	{
		mBoxComp->setColor(newColor);
	}

private:

	ObjectRef<BoxComponent> mBoxComp;

};

class MoveComponent : public Component
{
public:

	MoveComponent()
	{
		bCanCreateTransform = false;
	}

private:

	bool awake()override
	{
		mActOwner = Cast<Actor>(getOwner());
		return mActOwner;
	}

	void update(double inDeltaTime)override
	{
		Vec3 axis
		{
			(KeyD.pressed() - KeyA.pressed()),
			(KeyUp.pressed() - KeyDown.pressed()),
			(KeyW.pressed() - KeyS.pressed())
		};

		if (axis.isZero())
			return;

		axis.normalize();

		const double speed = 10 * inDeltaTime;

		mActOwner->addLocalPosition(axis * speed);
	}

private:

	ObjectRef<Actor> mActOwner;

};

class MyLevel : public Level
{
private:

	bool awake()override
	{
		mAct1 = createActor<BoxActor>(U"Act1");
		{
			mAct1->attachComponent<MoveComponent>();
			mAct1->setLocalPosition(Vec3{ 5,0,0 });
			mAct1->setLocalScale(Vec3{ 1,1,2 });
			mAct1->setExtent(Vec3{ 1,1,1 });
			mAct1->setColor(Palette::Orange);
		}

		mAct2 = createActor<BoxActor>(U"Act2");
		{
			mAct2->setLocalPosition(Vec3{ 0,0,0 });
			mAct2->setExtent(Vec3{ 1,1,1 });
			mAct2->setColor(Palette::Aliceblue);
		}

		return true;
	}

	void update(double)override
	{
		mAct2->LookAtForActor(mAct1);
	}

private:

	ObjectRef<BoxActor> mAct1;
	ObjectRef<BoxActor> mAct2;

};

class Base
{
protected:

	void Func1() {}
	void Func2() {}

};

void Main()
{

	ObjectPtr<World> world = CreateObjectClass<World>()();
	{
		world->registerLevel<MyLevel>(U"Lv1");
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
