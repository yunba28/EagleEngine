#include <Siv3D.hpp> // OpenSiv3D v0.6.9

#include <GameFramework/World.hpp>
#include <GameFramework/Actor.hpp>
#include <GameFramework/Component.hpp>
#include <Core/WorldObjectSubSystem.hpp>
#include <Render/Renderer.hpp>
#include <Components/Camera2DComponent.hpp>

using namespace eagle;

class CircleRenderer : public Renderer
{
private:

	void draw()const override
	{
		Circle{ getWorldPosition().xy(),mRadius}.draw(mColor);
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

	Color mColor = Palette::White;

	double mRadius = 20;

};

class QuadComponent : public Renderer
{
private:

	void draw()const override
	{
		Vec2 pos = getWorldPosition().xy();
		RectF rect{ pos,mExtent * 2 };
		auto rot = getWorldRotation();
		Vec2 dir = (Vec3{ 1,0,0 }*rot).xy();
		rect.rotated(dir.getAngle()).draw(mColor);
	}

public:

	void setColor(const Color& newColor)
	{
		mColor = newColor;
	}

	void setExtent(const Vec2& newExtent)
	{
		mExtent = newExtent;
	}

private:

	Color mColor = Palette::White;
	Vec2 mExtent = { 25,25 };

};

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
		Vec3 pos = getWorldPosition();
		Box box{ pos,mExtent * 2 };
		box.draw(getWorldRotation(), mColor);
	}

public:

	void setColor(const Color& newColor)
	{
		mColor = newColor.removeSRGBCurve();
	}

	void setExtent(const Vec3& newExtent)
	{
		mExtent = newExtent;
	}

private:

	Color mColor = Palette::White.removeSRGBCurve();
	Vec3 mExtent = { 0.5,0.5,0.5 };

};

class PlayableQuadActor : public Actor
{
private:

	bool awake()override
	{
		mQC = attachComponent<BoxComponent>();
		mQC->setExtent(Vec3{ 1,1,1 });
		mQC2 = createComponent<BoxComponent>();
		mQC2->attachToComponent(mQC);
		mQC2->setLocalPosition(Vec3{ 3,0,0 });
		mQC2->setExtent(Vec3{ 0.5,0.5,0.5 });
		return true;
	}

	void update(double inDeltaTime)override
	{
		const double angleVelocity = 120 * inDeltaTime;

		Vec3 euler
		{
			(KeyQ.pressed() - KeyA.pressed()) * angleVelocity,
			(KeyW.pressed() - KeyS.pressed()) * angleVelocity,
			(KeyE.pressed() - KeyD.pressed()) * angleVelocity
		};

		euler = ToRadians(euler);

		if (KeyR.down())
		{
			setLocalRotation(Vec3::Zero());
		}

		addLocalRotation(euler);
	}

public:

	void setColor(const Color& newColor)
	{
		mQC->setColor(newColor);
	}

private:
	
	ObjectRef<BoxComponent> mQC;
	ObjectRef<BoxComponent> mQC2;

};

class MyLevel : public Level
{
private:

	bool awake()override
	{
		createActor<PlayableQuadActor>(U"MyActor")
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
		createActor<PlayableQuadActor>(U"MyActor")
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
