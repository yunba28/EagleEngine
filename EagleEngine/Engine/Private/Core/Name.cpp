#include "Core/Name.hpp"

#include "Container/HashTable.hpp"

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

	int32 NameBase::Make(const String& inName, Name& outName)
	{
		// 設定する名前のID(Hash)を取得
		NameID id = inName.hash();

		// 新たに生成される名前データ
		NameRow newNameRow;

		if (Exist(id))
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
				return id;
			}
			// 番号を足した新たなデータを生成
			{
				newNameRow.name = U"{}_{}"_fmt(baseNameRow.name, baseNameRow.count);
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

		return id;
	}

	void NameBase::Release(Name& inName)
	{
		if (Exist(inName.mNameID))
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
				// ベースの名前なら再使用の可能性があるのでテーブルからの削除は行わない
				if (nameRow.name.hash() == nameRow.base)
				{
					nameRow.max = 0;
				}
				// ベースの名前でなければテーブルから削除する
				else
				{
					
					gNameTable.erase(inName.mNameID);
				}
				// Nameに無効値を入れる
				inName.mNameID = 0;
			}
		}
	}

	bool NameBase::Exist(NameID inNameID)
	{
		return gNameTable.contains(inNameID);
	}

	bool NameBase::Exist(const String& inName)
	{
		return Exist(inName.hash());
	}

	bool NameBase::Find(NameID inNameID, String& outName)
	{
		if (Exist(inNameID))
		{
			if (NameRow nameRow = GetNameRow(inNameID);nameRow.count > 0)
			{
				outName = nameRow.name;
				return true;
			}
		}
		return false;
	}

	bool NameBase::Find(const String& inName, String& outName)
	{
		return Find(inName.hash(), outName);
	}
}
