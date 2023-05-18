#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/World.hpp>
#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Render/Renderer.hpp>
#include <Components/Camera2DComponent.hpp>

using namespace eagle;

class PositionComponent : public Component
{
private:

	bool awake()override
	{
		setUpdateEnable(false);
		return true;
	}

public:

	void setPos(Vec2 newPos)
	{
		mPosition = newPos;
	}

	Vec2 getPos()const
	{
		return mPosition;
	}

	void addPos(Vec2 inVec)
	{
		mPosition += inVec;
	}

private:

	Vec2 mPosition = { 0,0 };

};

class CircleRenderer : public Renderer
{
private:

	void start()override
	{
		mPosComp = Cast<Actor>(getOwner())->findComponent<PositionComponent>();
	}

	void draw()const override
	{
		Circle{ mPosComp->getPos(),mRadius }.draw(mColor);
	}

public:

	void setColor(const Color& newColor)
	{
		mColor = newColor;
	}

	const Color& getColor()const
	{
		return mColor;
	}

	void setRadius(double newRadius)
	{
		mRadius = newRadius;
	}

	double getRadius()const
	{
		return mRadius;
	}

private:

	ObjectRef<PositionComponent> mPosComp = nullptr;

	Color mColor = Palette::White;

	double mRadius = 20;

};

class MoveComponent : public Component
{
private:

	void start()override
	{
		mPosComp = Cast<Actor>(getOwner())->findComponent<PositionComponent>();
	}

	void update(double inDeltaTime)override
	{
		const double velocity = 300.0 * inDeltaTime;

		Vec2 axis
		{
			(KeyRight.pressed() - KeyLeft.pressed()),
			(KeyDown.pressed() - KeyUp.pressed())
		};

		if (!axis.isZero())
		{
			axis.normalize();
			mPosComp->addPos(axis * velocity);
		}
	}

private:

	ObjectRef<PositionComponent> mPosComp;

};

class PlayableCircleActor : public Actor
{
private:

	bool awake()override
	{
		attachComponent<PositionComponent>();
		attachComponent<MoveComponent>();
		mCR = attachComponent<CircleRenderer>();
		return true;
	}

public:

	void setColor(const Color& newColor)
	{
		mCR->setColor(newColor);
	}

private:

	ObjectRef<CircleRenderer> mCR;

};

class MyLevel : public Level
{
private:

	bool awake()override
	{
		createActor<PlayableCircleActor>(U"MyActor")
			->setColor(Palette::Orange);
		return true;
	}

	void update(double)override
	{
		if (KeyEnter.down())
		{
			getWorld()->changeLevel(U"MyLevel2");
		}
	}

};

class MyLevel2 : public Level
{
private:

	bool awake()override
	{
		createActor<PlayableCircleActor>(U"MyActor")
			->setColor(Palette::Aliceblue);
		return true;
	}

	void update(double)override
	{
		if (KeyEnter.down())
		{
			getWorld()->changeLevel(U"MyLevel");
		}
	}
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
