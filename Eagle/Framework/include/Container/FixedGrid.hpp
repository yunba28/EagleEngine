#pragma once

#include <Utility/InitializerList.hpp>

namespace EagleEngine
{

	template<class Type, size_t Row, size_t Column>
	class FixedGrid
	{
	public:

		using value_type = Type;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using pointer = Type*;
		using const_pointer = const Type*;
		using reference = Type&;
		using const_reference = const Type&;

		using iterator = std::_Array_iterator<Type, Row * Column>;
		using const_iterator = std::_Array_const_iterator<Type, Row * Column>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		FixedGrid() = default;

		FixedGrid(InitializerList<Type> _grid)
			: mElem()
		{
			std::memmove(mElem, _grid.begin(), Row * Column * sizeof(Type));
		}

		FixedGrid(InitializerList<InitializerList<Type>> _grid)
			: mElem()
		{
			int offset = 0;

			for (auto&& row : _grid)
			{
				std::memmove(mElem + offset, row.begin(), Column * sizeof(Type));
				offset += Column;
			}
		}

		iterator begin() noexcept
		{
			return iterator(mElem, 0);
		}

		const_iterator begin() const noexcept
		{
			return const_iterator(mElem, 0);
		}

		iterator end() noexcept
		{
			return iterator(mElem, Row * Column);
		}

		const_iterator end() const noexcept
		{
			return const_iterator(mElem, Row * Column);
		}

		Type* operator[](size_t _idx)
		{
			return mElem[_idx];
		}

	private:

		Type mElem[Row * Column];

	};


}
