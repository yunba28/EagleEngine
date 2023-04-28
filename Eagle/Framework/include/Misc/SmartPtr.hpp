#pragma once

#include <memory>

namespace EagleEngine
{

	template<class Type, class Deleter = std::default_delete<Type>>
	class UniquePtr : protected std::unique_ptr<Type, Deleter>
	{
	public:

		using Base_t = std::unique_ptr<Type, Deleter>;

		using Base_t::unique_ptr;
		using Base_t::operator=;
		using Base_t::swap;
		using Base_t::reset;
		using Base_t::operator*;
		using Base_t::operator->;
		using Base_t::operator bool;

		bool invalid()const noexcept;

	};

	template<class Type, class Deleter>
	class UniquePtr<Type[], Deleter> : protected std::unique_ptr<Type[], Deleter>
	{
	public:

		using Base_t = std::unique_ptr<Type[], Deleter>;

		using Base_t::unique_ptr;
		using Base_t::operator=;
		using Base_t::swap;
		using Base_t::reset;
		using Base_t::operator[];
		using Base_t::operator bool;

		bool invalid()const noexcept;

	};

	template<class Type, class Deleter>
	bool UniquePtr<Type, Deleter>::invalid() const noexcept
	{
		return not this->operator bool();
	}

	template<class Type, class ...Args, std::enable_if_t<not std::is_array_v<Type>, int> = 0>
	UniquePtr<Type> MakeUnique(Args&& ...args)
	{
		return UniquePtr<Type>{new Type{ std::forward<Args>(args)... }};
	}

	template<class Type, std::enable_if_t<std::is_array_v<Type> and std::extent_v<Type> == 0, int> = 0>
	UniquePtr<Type> MakeUnique(const size_t size)
	{
		using Element = std::remove_extent_t<Type>;
		return UniquePtr<Type>{new Element[size]()};
	}

	template<class Type, class ...Args, std::enable_if_t<std::extent_v<Type> != 0, int> = 0>
	void MakeUnique(Args&&...) = delete;

	template<class Type>
	class SharedPtr;

	template<class Type>
	class WeakPtr;

	template<class Type>
	class PtrHandle;

	template<class Type>
	class SharedPtr final
	{
	public:

		explicit SharedPtr()noexcept;

		explicit SharedPtr(std::nullptr_t)noexcept;

		template<class OtherType>
		explicit SharedPtr(OtherType* _ptr);

		template<class OtherType>
		explicit SharedPtr(const SharedPtr<OtherType>& _other);

		template<class OtherType, class Deleter>
		SharedPtr(OtherType* _ptr, Deleter _deleter);

		template<class OtherType, class Deleter, class Allocator>
		SharedPtr(OtherType* _ptr, Deleter _deleter, Allocator _allocator);

		template<class Deleter>
		SharedPtr(std::nullptr_t, Deleter _deleter);

		template<class Deleter, class Allocator>
		SharedPtr(std::nullptr_t, Deleter _deleter, Allocator _allocator);

		SharedPtr(const SharedPtr<Type>&) = default;

		SharedPtr(SharedPtr<Type>&&)noexcept = default;

		template<class OtherType>
		SharedPtr(SharedPtr<OtherType>&& _other);

		template<class OtherType>
		SharedPtr(const WeakPtr<OtherType>& _weak);

		~SharedPtr() = default;

		SharedPtr<Type>& operator=(const SharedPtr<Type>&) = default;

		template<class OtherType>
		SharedPtr<Type>& operator=(const SharedPtr<OtherType>& _other);

		SharedPtr<Type>& operator=(SharedPtr<Type>&&) = default;

		template<class OtherType>
		SharedPtr<Type>& operator=(SharedPtr<OtherType>&& _other);

		SharedPtr<Type>& operator=(std::nullptr_t)noexcept;

		void swap(SharedPtr& _other)noexcept;

		void reset()noexcept;

		template<class OtherType>
		void reset(OtherType* _ptr);

		template<class OtherType, class Deleter>
		void reset(OtherType* _ptr, Deleter _deleter);

		template<class OtherType, class Deleter, class Allocator>
		void reset(OtherType* _ptr, Deleter _deleter, Allocator _allocator);

		WeakPtr<Type> weak()const noexcept;

		template<class OtherType>
		WeakPtr<OtherType> weak()const;

		PtrHandle<Type> lock()const noexcept;

		template<class OtherType>
		PtrHandle<OtherType> lock()const;

		Type& operator*()noexcept;

		const Type& operator*()const noexcept;

		Type* operator->()const noexcept;

		bool invalid()const noexcept;

		explicit operator bool()const noexcept;

	private:

		SharedPtr(std::shared_ptr<Type>&& _shared)noexcept;

	private:

		std::shared_ptr<Type> mPtr;

		template<class OtherType>
		friend class SharedPtr;
		template<class OtherType>
		friend class WeakPtr;

		template<class Type, class ...Args>
		friend std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> MakeShared(Args&& ...args);

		template<class Type, class Allocator, class ...Args>
		friend std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> AllocateShared(const Allocator& allocator, Args&& ...args);

		template<class To, class From>
		friend SharedPtr<To> Cast(const SharedPtr<From>&);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, std::nullptr_t);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, std::nullptr_t);
	};

	template<class Type>
	class WeakPtr final
	{
	public:

		WeakPtr()noexcept;

		WeakPtr(std::nullptr_t)noexcept;

		WeakPtr(const WeakPtr<Type>&)noexcept = default;

		template<class OtherType>
		WeakPtr(const WeakPtr<OtherType>& _other)noexcept;

		WeakPtr(WeakPtr<Type>&&)noexcept = default;

		template<class OtherType>
		WeakPtr(WeakPtr<OtherType>&& _other)noexcept;

		template<class OtherType>
		WeakPtr(const SharedPtr<OtherType>& _shared);

		WeakPtr& operator=(const WeakPtr&)noexcept = default;

		template<class OtherType>
		WeakPtr& operator=(const WeakPtr<OtherType>& _other)noexcept;

		WeakPtr& operator=(WeakPtr&&)noexcept = default;

		template<class OtherType>
		WeakPtr& operator=(WeakPtr<OtherType>&& _other)noexcept;

		void reset()noexcept;

		void swap(WeakPtr<Type>& _other)noexcept;

		PtrHandle<Type> lock()const noexcept;

		template<class OtherType>
		PtrHandle<OtherType> lock()const noexcept;

		bool expired() const noexcept;

		bool invalid()const noexcept;

		explicit operator bool()const noexcept;

	private:

		std::weak_ptr<Type> mWeak;

		template<class OtherType>
		friend class SharedPtr;
		template<class OtherType>
		friend class WeakPtr;
		template<class OtherType>
		friend class PtrHandle;

		template<class To, class From>
		friend WeakPtr<To> Cast(const WeakPtr<From>&);

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, std::nullptr_t);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, std::nullptr_t);
	};

	template<class Type>
	class PtrHandle final
	{
	public:

		PtrHandle() = delete;

		PtrHandle(const PtrHandle&) = delete;

		PtrHandle(PtrHandle&&) = default;

		PtrHandle& operator=(const PtrHandle&) = delete;

		PtrHandle& operator=(PtrHandle&&) = default;

		~PtrHandle()noexcept = default;

		Type& operator*()noexcept;

		const Type& operator*()const noexcept;

		Type* operator->()const noexcept;

		bool invalid()const noexcept;

		explicit operator bool()const noexcept;

	private:

		std::shared_ptr<Type> mShared;

		template<class OtherType>
		friend class SharedPtr;
		template<class OtherType>
		friend class WeakPtr;
		template<class OtherType>
		friend class PtrHandle;

		PtrHandle(const std::shared_ptr<Type>& _shared)noexcept;

		template<class OtherType>
		PtrHandle(const std::shared_ptr<OtherType>& _shared)noexcept;

		template<class Type>
		friend bool operator==(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator==(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator==(const PtrHandle<Type>& a, std::nullptr_t);

		template<class Type>
		friend bool operator!=(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator!=(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, const PtrHandle<Type>& b);

		template<class Type>
		friend bool operator!=(const PtrHandle<Type>& a, std::nullptr_t);
	};

	template<class Type, class ...Args>
	std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> MakeShared(Args&& ...args);

	template<class Type, class Allocator, class ...Args>
	std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> AllocateShared(const Allocator& allocator, Args&& ...args);

	template<class To, class From>
	SharedPtr<To> Cast(const SharedPtr<From>& from);

	template<class To, class From>
	WeakPtr<To> Cast(const WeakPtr<From>& from);

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, std::nullptr_t);

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, std::nullptr_t);

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, std::nullptr_t);

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const SharedPtr<Type>& b);

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const WeakPtr<Type>& b);

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const PtrHandle<Type>& b);

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, std::nullptr_t);

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, std::nullptr_t);

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, std::nullptr_t);

	template<class Type>
	inline SharedPtr<Type>::SharedPtr()noexcept
		: mPtr()
	{

	}

	template<class Type>
	inline SharedPtr<Type>::SharedPtr(std::nullptr_t)noexcept
		: mPtr()
	{

	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>::SharedPtr(OtherType* _ptr)
		: mPtr(_ptr)
	{

	}

	template<class Type>
	template<class OtherType, class Deleter>
	inline SharedPtr<Type>::SharedPtr(OtherType* _ptr, Deleter _deleter)
		: mPtr(_ptr, _deleter)
	{

	}

	template<class Type>
	template<class OtherType, class Deleter, class Allocator>
	inline SharedPtr<Type>::SharedPtr(OtherType* _ptr, Deleter _deleter, Allocator _allocator)
		: mPtr(_ptr, _deleter, _allocator)
	{

	}

	template<class Type>
	template<class Deleter>
	inline SharedPtr<Type>::SharedPtr(std::nullptr_t, Deleter _deleter)
		: mPtr(std::nullptr_t, _deleter)
	{

	}

	template<class Type>
	template<class Deleter, class Allocator>
	inline SharedPtr<Type>::SharedPtr(std::nullptr_t, Deleter _deleter, Allocator _allocator)
		: mPtr(std::nullptr_t, _deleter, _allocator)
	{

	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>::SharedPtr(const SharedPtr<OtherType>& _other)
		: mPtr(_other.mPtr)
	{

	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>::SharedPtr(SharedPtr<OtherType>&& _other)
		: mPtr(std::forward<decltype(_other.mPtr)>(_other.mPtr))
	{

	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>::SharedPtr(const WeakPtr<OtherType>& _weak)
		: mPtr(_weak.mWeak)
	{

	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>& SharedPtr<Type>::operator=(const SharedPtr<OtherType>& _other)
	{
		mPtr = _other.mPtr;
		return *this;
	}

	template<class Type>
	template<class OtherType>
	inline SharedPtr<Type>& SharedPtr<Type>::operator=(SharedPtr<OtherType>&& _other)
	{
		mPtr = std::move(_other.mPtr);
		return *this;
	}

	template<class Type>
	inline SharedPtr<Type>& SharedPtr<Type>::operator=(std::nullptr_t)noexcept
	{
		mPtr.reset();
		return *this;
	}

	template<class Type>
	inline void SharedPtr<Type>::swap(SharedPtr& _other)noexcept
	{
		mPtr.swap(_other.mPtr);
	}

	template<class Type>
	inline void SharedPtr<Type>::reset()noexcept
	{
		mPtr.reset();
	}

	template<class Type>
	template<class OtherType>
	inline void SharedPtr<Type>::reset(OtherType* _ptr)
	{
		mPtr.reset(_ptr);
	}

	template<class Type>
	template<class OtherType, class Deleter>
	inline void SharedPtr<Type>::reset(OtherType* _ptr, Deleter _deleter)
	{
		mPtr.reset(_ptr, _deleter);
	}

	template<class Type>
	template<class OtherType, class Deleter, class Allocator>
	inline void SharedPtr<Type>::reset(OtherType* _ptr, Deleter _deleter, Allocator _allocator)
	{
		mPtr.reset(_ptr, _deleter, _allocator);
	}

	template<class Type>
	inline WeakPtr<Type> SharedPtr<Type>::weak()const noexcept
	{
		return WeakPtr<Type>{*this};
	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<OtherType> SharedPtr<Type>::weak()const
	{
		return WeakPtr<OtherType>{Cast<OtherType>(*this)};
	}

	template<class Type>
	inline PtrHandle<Type> SharedPtr<Type>::lock()const noexcept
	{
		return PtrHandle<Type>{mPtr};
	}

	template<class Type>
	template<class OtherType>
	inline PtrHandle<OtherType> SharedPtr<Type>::lock()const
	{
		return PtrHandle<OtherType>{Cast<OtherType>(*this).mPtr};
	}

	template<class Type>
	inline Type& SharedPtr<Type>::operator*()noexcept
	{
		return *mPtr;
	}

	template<class Type>
	inline const Type& SharedPtr<Type>::operator*()const noexcept
	{
		return *mPtr;
	}

	template<class Type>
	inline Type* SharedPtr<Type>::operator->()const noexcept
	{
		return mPtr.get();
	}

	template<class Type>
	inline bool SharedPtr<Type>::invalid()const noexcept
	{
		return not mPtr.operator bool();
	}

	template<class Type>
	inline SharedPtr<Type>::operator bool()const noexcept
	{
		return mPtr.operator bool();
	}

	template<class Type>
	inline SharedPtr<Type>::SharedPtr(std::shared_ptr<Type>&& _shared)noexcept
		: mPtr(std::forward<std::shared_ptr<Type>>(_shared))
	{

	}

	template<class Type>
	inline WeakPtr<Type>::WeakPtr()noexcept
		: mWeak()
	{

	}

	template<class Type>
	inline WeakPtr<Type>::WeakPtr(std::nullptr_t)noexcept
		: mWeak()
	{

	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<Type>::WeakPtr(const WeakPtr<OtherType>& _other)noexcept
		: mWeak(_other.mWeak)
	{

	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<Type>::WeakPtr(WeakPtr<OtherType>&& _other)noexcept
		: mWeak(std::forward<decltype(_other.mWeak)>(_other.mWeak))
	{

	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<Type>::WeakPtr(const SharedPtr<OtherType>& _other)
		: mWeak(_other.mPtr)
	{

	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<Type>& WeakPtr<Type>::operator=(const WeakPtr<OtherType>& _other) noexcept
	{
		mWeak = _other.mWeak;
		return *this;
	}

	template<class Type>
	template<class OtherType>
	inline WeakPtr<Type>& WeakPtr<Type>::operator=(WeakPtr<OtherType>&& _other) noexcept
	{
		mWeak = std::forward<decltype(_other.mWeak)>(_other.mWeak);
		return *this;
	}

	template<class Type>
	inline void WeakPtr<Type>::reset()noexcept
	{
		mWeak.reset();
	}

	template<class Type>
	inline void WeakPtr<Type>::swap(WeakPtr<Type>& _other)noexcept
	{
		mWeak.swap(_other.mWeak);
	}

	template<class Type>
	inline PtrHandle<Type> WeakPtr<Type>::lock()const noexcept
	{
		return PtrHandle<Type>{mWeak.lock()};
	}

	template<class Type>
	template<class OtherType>
	inline PtrHandle<OtherType> WeakPtr<Type>::lock()const noexcept
	{
		return PtrHandle<OtherType>{Cast<OtherType>(*this).mWeak.lock()};
	}

	template<class Type>
	inline bool WeakPtr<Type>::expired()const noexcept
	{
		return mWeak.expired();
	}

	template<class Type>
	inline bool WeakPtr<Type>::invalid()const noexcept
	{
		return mWeak.expired();
	}

	template<class Type>
	inline WeakPtr<Type>::operator bool()const noexcept
	{
		return not mWeak.expired();
	}

	template<class Type>
	inline Type& PtrHandle<Type>::operator*()noexcept
	{
		return *mShared;
	}

	template<class Type>
	inline const Type& PtrHandle<Type>::operator*()const noexcept
	{
		return *mShared;
	}

	template<class Type>
	inline Type* PtrHandle<Type>::operator->()const noexcept
	{
		return mShared.get();
	}

	template<class Type>
	inline bool PtrHandle<Type>::invalid()const noexcept
	{
		return not mShared.operator bool();
	}

	template<class Type>
	inline PtrHandle<Type>::operator bool()const noexcept
	{
		return mShared.operator bool();
	}

	template<class Type>
	inline PtrHandle<Type>::PtrHandle(const std::shared_ptr<Type>& _shared)noexcept
		: mShared(_shared)
	{

	}

	template<class Type>
	template<class OtherType>
	inline PtrHandle<Type>::PtrHandle(const std::shared_ptr<OtherType>& _shared)noexcept
		: mShared(_shared)
	{

	}

	template<class Type, class ...Args>
	std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> MakeShared(Args&& ...args)
	{
		return SharedPtr<Type>{std::make_shared<Type>(std::forward<Args>(args)...)};
	}

	template<class Type, class Allocator, class ...Args>
	std::enable_if_t<not std::is_array_v<Type>, SharedPtr<Type>> AllocateShared(const Allocator& allocator, Args&& ...args)
	{
		return SharedPtr<Type>{std::allocate_shared<Type>(allocator, std::forward<Args>(args)...)};
	}

	template<class To, class From>
	SharedPtr<To> Cast(const SharedPtr<From>& from)
	{
		if constexpr (std::is_same_v<To, From>)
		{
			return from;
		}
		else if constexpr (std::is_base_of_v<To, From>)
		{
			return SharedPtr<To>{std::static_pointer_cast<To>(from.mPtr)};
		}
		else if constexpr (std::is_base_of_v<From, To>)
		{
			return SharedPtr<To>{std::dynamic_pointer_cast<To>(from.mPtr)};
		}
		else if constexpr (std::is_convertible_v<To*, From*>)
		{
			return SharedPtr<To>{std::reinterpret_pointer_cast<To>(from.mPtr)};
		}
		else
		{
			throw std::bad_cast{ "bad-cast" };
		}
	}

	template<class To, class From>
	WeakPtr<To> Cast(const WeakPtr<From>& from)
	{
		if constexpr (std::is_same_v<To, From>)
		{
			return from;
		}

		WeakPtr<To> to{};
		auto sp = from.mWeak.lock();

		if constexpr (std::is_base_of_v<To, From>)
		{
			to.mWeak = std::static_pointer_cast<To>(sp);
			return to;
		}
		else if constexpr (std::is_base_of_v<From, To>)
		{
			to.mWeak = std::dynamic_pointer_cast<To>(sp);
			return to;
		}
		else if constexpr (std::is_convertible_v<To*, From*>)
		{
			to.mWeak = std::reinterpret_pointer_cast<To>(sp);
			return to;
		}
		else
		{
			throw std::bad_cast{ "bad-cast" };
		}
	}

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mPtr == b.mPtr;
	}

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mPtr == b.mWeak.lock();
	}

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mPtr == b.mShared;
	}

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mWeak.lock() == b.mPtr;
	}

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mWeak.lock() == b.mWeak.lock();
	}

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mWeak.lock() == b.mShared;
	}

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mShared == b.mPtr;
	}

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mShared == b.mWeak.lock();
	}

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mShared == b.mShared;
	}

	template<class Type>
	bool operator==(const SharedPtr<Type>& a, std::nullptr_t)
	{
		return a.mPtr == nullptr;
	}

	template<class Type>
	bool operator==(const WeakPtr<Type>& a, std::nullptr_t)
	{
		return a.expired();
	}

	template<class Type>
	bool operator==(const PtrHandle<Type>& a, std::nullptr_t)
	{
		return a.mShared == nullptr;
	}

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mPtr != b.mPtr;
	}

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mPtr != b.mWeak.lock();
	}

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mPtr != b.mShared;
	}

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mWeak.lock() != b.mPtr;
	}

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mWeak.lock() != b.mWeak.lock();
	}

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mWeak.lock() != b.mShared;
	}

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const SharedPtr<Type>& b)
	{
		return a.mShared != b.mPtr;
	}

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const WeakPtr<Type>& b)
	{
		return a.mShared != b.mWeak.lock();
	}

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, const PtrHandle<Type>& b)
	{
		return a.mShared != b.mShared;
	}

	template<class Type>
	bool operator!=(const SharedPtr<Type>& a, std::nullptr_t)
	{
		return a.mPtr != nullptr;
	}

	template<class Type>
	bool operator!=(const WeakPtr<Type>& a, std::nullptr_t)
	{
		return !a.expired();
	}

	template<class Type>
	bool operator!=(const PtrHandle<Type>& a, std::nullptr_t)
	{
		return a.mShared != nullptr;
	}

}
