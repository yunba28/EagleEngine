#include <Core/Name.hpp>

#include <CoreUtility.hpp>
#include <Container/HashTable.hpp>

namespace eagle
{
	HashTable<NameID, NameRow> gNameTable
	{
		{0,{U"",0,0,0}}
	};

	// 要素チェックはしない
	NameRow& GetNameRow(NameID inNameID)
	{
		return gNameTable.at(inNameID);
	}

	String NameBase::toString() const
	{
		const NameRow& nameRow = GetNameRow(mNameID);
		return (nameRow.count == 0) ? U""_s : nameRow.name;
	}

	String NameBase::getOriginalName() const
	{
		const NameRow& nameRow = GetNameRow(mNameID);
		const NameRow& baseNameRow = GetNameRow(nameRow.base);
		return (baseNameRow.count == 0) ? U""_s : baseNameRow.name;
	}

	String NameBase::getSequentialName() const
	{
		return toString();
	}

	int32 NameBase::Make(const String& inName, Name& outName)
	{
		Release(outName);

		// 設定する名前のID(Hash)を取得
		NameID id = inName.hash();

		// 新たに生成される名前データ
		NameRow newNameRow;

		if (Exist(id, false))
		{
			// 既に存在するデータの参照、カウントアップ
			NameRow& baseNameRow = GetNameRow(id);
			{
				baseNameRow.count += 1;
				baseNameRow.max += 1;
			}
			// 再使用された場合
			if (baseNameRow.count == 1)
			{
				outName.mNameID = id;
				return baseNameRow.max;
			}
			// 番号を足した新たなデータを生成
			{
				newNameRow.name = U"{}_{}"_fmt(baseNameRow.name, baseNameRow.max);
				newNameRow.base = id;
				newNameRow.count = 1;
				newNameRow.max = 1;
			}
			// idを新たな名前のものに更新
			id = newNameRow.name.hash();
		}
		else
		{
			// データの生成
			newNameRow.name = inName;
			newNameRow.base = id;
			newNameRow.count = 1;
			newNameRow.max = 1;
		}

		// テーブルへの登録
		gNameTable.emplace(id, newNameRow);
		// 名前の値を更新
		outName.mNameID = id;

		return newNameRow.max;
	}

	void NameBase::Release(Name& inName)
	{
		if (Exist(inName.mNameID, false))
		{
			NameRow& nameRow = GetNameRow(inName.mNameID);

			// すでに使用されていない
			if (nameRow.count == 0)
			{
				inName.mNameID = 0;
				return;
			}

			// 使用数をデクリメントする
			nameRow.count = Max(nameRow.count - 1, 0);
			// 名前の使用数がゼロになった
			if (nameRow.count == 0)
			{
				if (nameRow.name.hash() == nameRow.base)
				{
					nameRow.max = 0;
				}
				else
				{
					NameRow& baseNameRow = GetNameRow(nameRow.base);
					// ベースデータの使用数をデクリメントします
					baseNameRow.count = Max(baseNameRow.count - 1, 0);
					// ベースデータの使用数がゼロになった
					if (baseNameRow.count == 0)
					{
						baseNameRow.max = 0;
					}
				}
				// Nameに無効値を入れる
				inName.mNameID = 0;
			}
		}
	}

	bool NameBase::Exist(NameID inNameID, bool inConsiderationUseFlag)
	{
		bool exist = gNameTable.contains(inNameID);
		if (exist && inConsiderationUseFlag)
		{
			const NameRow& nameRow = GetNameRow(inNameID);
			return nameRow.count > 0;
		}
		return exist;
	}

	bool NameBase::Exist(const String& inName, bool inConsiderationUseFlag)
	{
		return Exist(inName.hash(), inConsiderationUseFlag);
	}

	bool NameBase::Find(NameID inNameID, String& outName)
	{
		if (Exist(inNameID, true))
		{
			outName = GetNameRow(inNameID).name;
			return true;
		}
		return false;
	}

	bool NameBase::Find(const String& inName, String& outName)
	{
		return Find(inName.hash(), outName);
	}
}
