#include <String/NameView.hpp>
#include <String/Name.hpp>

namespace EagleEngine
{
	NameView::NameView() noexcept
		: mNameID(0)
	{
	}

	NameView::NameView(const NameView& _other) noexcept
		: mNameID(_other.mNameID)
	{
	}

	NameView::NameView(const Name& _other) noexcept
		: mNameID(_other.id())
	{
	}

	NameView::NameView(NameView&& _other) noexcept
		: mNameID(std::forward<NameID>(_other.mNameID))
	{
		_other.mNameID = 0;
	}

	NameView& NameView::operator=(const NameView& _other) noexcept
	{
		mNameID = _other.mNameID;
		return *this;
	}

	NameView& NameView::operator=(const Name& _other) noexcept
	{
		mNameID = _other.id();
		return *this;
	}

	NameView& NameView::operator=(NameView&& _other) noexcept
	{
		mNameID = std::forward<NameID>(_other.mNameID);
		return *this;
	}

	NameID NameView::id() const noexcept
	{
		return mNameID;
	}

	uint64 NameView::hash() const noexcept
	{
		return mNameID;
	}

	String NameView::toString() const
	{
		if (!Name::Used(mNameID))
			return U""_s;

		return Name::sNameTable.at(mNameID).name;
	}

	String NameView::getOriginalName() const
	{
		if (!Name::Used(mNameID))
			return U""_s;

		const Name::Param& prm = Name::sNameTable.at(mNameID);

		if (prm.base == mNameID)
		{
			return prm.name;
		}
		else
		{
			return Name::sNameTable.at(prm.base).name;
		}
	}

	String NameView::getSequentialName() const
	{
		return toString();
	}

	bool NameView::empty() const noexcept
	{
		return mNameID == 0;
	}

	bool NameView::operator==(const Name& _other) const noexcept
	{
		return mNameID == _other.mNameID;
	}

	bool NameView::operator==(const NameView& _other) const noexcept
	{
		return mNameID == _other.mNameID;
	}

	bool NameView::operator==(const String& _other) const noexcept
	{
		return mNameID == _other.hash();
	}

	bool NameView::operator!=(const Name& _other) const noexcept
	{
		return mNameID != _other.mNameID;
	}

	bool NameView::operator!=(const NameView& _other) const noexcept
	{
		return mNameID != _other.mNameID;
	}

	bool NameView::operator!=(const String& _other) const noexcept
	{
		return mNameID != _other.hash();
	}

	NameView::operator bool() const noexcept
	{
		return mNameID != 0;
	}

	NameView::NameView(NameID _id) noexcept
		: mNameID(_id)
	{
	}
}
