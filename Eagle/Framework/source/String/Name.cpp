#include <String/Name.hpp>

namespace EagleEngine
{
	HashTable<NameID, Name::Param> Name::sNameTable
	{
		{ 0,Param{} }
	};

	Name::Name()
		: mNameID(0)
	{
	}

	Name::Name(const char32* _name)
		: mNameID(Name::MakeID(_name))
	{
	}

	Name::Name(const String& _name)
		: mNameID(Name::MakeID(_name))
	{
	}

	Name::~Name()
	{
		Name::Release(*this);
	}

	Name::Name(Name&& _other) noexcept
		: mNameID(0)
	{
		Name::Release(*this);
		mNameID = Name::ExchangeID(std::forward<Name>(_other));
	}

	Name& Name::operator=(Name&& _other) noexcept
	{
		Name::Release(*this);
		mNameID = Name::ExchangeID(std::forward<Name>(_other));
		return *this;
	}

	NameID Name::id() const noexcept
	{
		return mNameID;
	}

	uint64 Name::hash() const noexcept
	{
		return mNameID;
	}

	String Name::toString() const
	{
		if (!Name::Used(mNameID))
			return U""_s;

		return Name::sNameTable.at(mNameID).name;
	}

	String Name::getOriginalName() const
	{
		if (!Name::Used(mNameID))
			return U""_s;

		const Param& prm = Name::sNameTable.at(mNameID);

		if (prm.base == mNameID)
		{
			return prm.name;
		}
		else
		{
			return Name::sNameTable.at(prm.base).name;
		}
	}

	String Name::getSequentialName() const
	{
		return toString();
	}

	bool Name::empty() const noexcept
	{
		return mNameID == 0;
	}

	Name Name::Make(const String& name)
	{
		return Name{ Name::MakeID(name)};
	}

	NameView Name::Find(const String& name)
	{
		if (Name::Used(name))
		{
			return NameView{ name.hash() };
		}

		return NameView{};
	}

	void Name::Release(Name& name)
	{
		// 名前が無い
		if (name.mNameID == 0)
			return;

		// IDが存在しない
		if (!Name::sNameTable.contains(name.mNameID))
			return;

		Param& param = Name::sNameTable.at(name.mNameID);
		param.count = Max(param.count - 1, 0);
		param.used = false;

		if (param.count == 0)
		{
			param.max = 0;
		}

		name.mNameID = 0;
	}

	void Name::ReleaseAll()
	{
		Name::sNameTable.clear();
	}

	bool Name::Used(NameID id)
	{
		if (id == 0)
			return false;

		if (!Name::sNameTable.contains(id))
			return false;

		return Name::sNameTable.at(id).used;
	}

	bool Name::Used(const String& name)
	{
		return Name::Used(name.hash());
	}

	bool Name::operator==(const Name& _other) const noexcept
	{
		return mNameID == _other.mNameID;
	}

	bool Name::operator==(const NameView& _other) const noexcept
	{
		return mNameID == _other.mNameID;
	}

	bool Name::operator==(const String& _other) const noexcept
	{
		return mNameID == _other.hash();
	}

	bool Name::operator!=(const Name& _other) const noexcept
	{
		return mNameID != _other.mNameID;
	}

	bool Name::operator!=(const NameView& _other) const noexcept
	{
		return mNameID != _other.mNameID;
	}

	bool Name::operator!=(const String& _other) const noexcept
	{
		return mNameID != _other.hash();
	}

	Name::operator bool() const noexcept
	{
		return mNameID != 0;
	}

	Name::Name(NameID _id) noexcept
		: mNameID(_id)
	{

	}

	NameID Name::MakeID(const String& name)
	{
		if (name == U"")
			return 0ULL;

		NameID id = name.hash();

		// 既に同じ名前が存在する
		if (sNameTable.contains(id))
		{
			// ベースパラメータを参照する
			Param& baseParam = sNameTable.at(id);

			// ベースパラメータが使われていれば新しい名前のパラメータを作成、登録する
			if (baseParam.used)
			{
				Param newParam
				{
					.name = baseParam.name + U"_{}"_fmt(baseParam.max),
					.base = baseParam.base,
					.count = 1,
					.max = 1,
					.used = true
				};

				// 生成した名前に変更が加わったのでhash/idを変更する
				id = newParam.name.hash();

				// テーブルに登録する
				sNameTable.emplace(id, newParam);
			}

			// ベースパラメータのカウンタを加算する
			{
				baseParam.count += 1;
				baseParam.max = Max(baseParam.count, baseParam.max);
				baseParam.used = true;
			}
		}
		// 新しい名前のパラメータを作成、登録する
		else
		{
			Param newParam
			{
				.name = name,
				.base = id,
				.count = 1,
				.max = 1,
				.used = true
			};

			// テーブルに登録する
			sNameTable.emplace(id, newParam);
		}

		return id;
	}

	NameID Name::ExchangeID(Name&& name)
	{
		NameID result = std::move(name.mNameID);
		name.mNameID = 0;
		return result;
	}
}
