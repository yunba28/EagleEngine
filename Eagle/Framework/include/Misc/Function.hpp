#pragma once

#include <Misc/SmartPtr.hpp>

namespace EagleEngine
{
	class BadFunctionCall : public std::exception
	{
	public:
		BadFunctionCall() :exception("bad function call") {}
		BadFunctionCall(const char* const _message) :exception(_message) {}
	};

	template<class>
	class Function;

	template<class ReturnType,class ...Args>
	class Function<ReturnType(Args...)>
	{
	private:

		class FunctionBase
		{
		public:
			virtual ~FunctionBase() = default;
			virtual ReturnType invoke(Args&&...)const = 0;
		};

		template<class FunctionType>
		class FunctionImpl : public FunctionBase
		{
		public:
			explicit FunctionImpl(FunctionType _function) : mFunctor(_function) {}
			ReturnType invoke(Args&&... _args) const final override
			{
				return mFunctor(std::forward<Args>(_args)...);
			}
		private:
			const FunctionType mFunctor;
		};

	public:

		using function_t = ReturnType(*)(Args...);

	public:

		Function()noexcept;

		Function(std::nullptr_t)noexcept;

		template<class Functor>
		Function(Functor _functor)
			: mFunction(MakeShared<FunctionImpl<Functor>>(_functor))
		{
		}

		Function(const Function&) = default;

		Function& operator=(const Function&) = default;

		Function(Function&&) = default;

		Function& operator=(Function&&) = default;

		Function& operator=(std::nullptr_t);

	public:

		ReturnType operator()(Args&&..._args) const
		{
			if (mFunction.invalid())
			{
				throw BadFunctionCall();
			}
			return mFunction->invoke(std::forward<Args>(_args)...);
		}

	private:

		SharedPtr<const FunctionBase> mFunction;

	};

	template<class...Args>
	class Function<void(Args...)>
	{
	private:

		class FunctionBase
		{
		public:
			virtual ~FunctionBase() = default;
			virtual void invoke(Args&&...)const = 0;
		};

		template<class FunctionType>
		class FunctionImpl : public FunctionBase
		{
		public:
			explicit FunctionImpl(FunctionType _function) : mFunctor(_function) {}
			void invoke(Args&&... _args) const final override
			{
				mFunctor(std::forward<Args>(_args)...);
			}
		private:
			const FunctionType mFunctor;
		};

	public:

		using function_t = void(*)(Args...);

	public:

		Function()noexcept;

		Function(std::nullptr_t)noexcept;

		template<class Functor>
		Function(Functor _functor)
			: mFunction(MakeShared<FunctionImpl<Functor>>(_functor))
		{
		}

		Function(const Function&) = default;

		Function& operator=(const Function&) = default;

		Function(Function&&) = default;

		Function& operator=(Function&&) = default;

		Function& operator=(std::nullptr_t);

		void operator()(Args&&..._args) const
		{
			if (mFunction.isNull())
			{
				throw BadFunctionCall();
			}
			mFunction->invoke(std::forward<Args>(_args)...);
		}

	private:

		SharedPtr<const FunctionBase> mFunction;

	};

	template<class ReturnType, class ...Args>
	inline Function<ReturnType(Args...)>::Function() noexcept
		: mFunction(nullptr)
	{
	}

	template<class ReturnType, class ...Args>
	inline Function<ReturnType(Args...)>::Function(std::nullptr_t) noexcept
		: mFunction(nullptr)
	{
	}

	template<class ReturnType, class ...Args>
	inline Function<ReturnType(Args...)>& Function<ReturnType(Args...)>::operator=(std::nullptr_t)
	{
		mFunction.reset();
		return *this;
	}

	template<class ClassType, class ReturnType, class ...Args>
	class Function<ReturnType(ClassType::*)(Args...)>
	{
	public:

		using function_t = ReturnType(ClassType::*)(Args...);

	public:

		Function()noexcept :mFunction(nullptr) {}

		Function(std::nullptr_t)noexcept :mFunction(nullptr) {}

		Function(function_t _function) :mFunction(_function) {}

		Function(const Function&) = default;

		Function& operator=(const Function&) = default;

		Function(Function&&) = default;

		Function& operator=(Function&&) = default;

		Function& operator=(std::nullptr_t) noexcept
		{
			mFunction = nullptr;
			return *this;
		}

		ReturnType operator()(ClassType* ptr, Args...args) const
		{
			if (mFunction == nullptr)
			{
				throw BadFunctionCall();
			}
			return (ptr->*mFunction)(std::forward<Args>(args)...);
		}

	private:

		function_t mFunction;

	};

	template<class ClassType, class ...Args>
	class Function<void(ClassType::*)(Args...)>
	{
	public:

		using function_t = void(ClassType::*)(Args...);

	public:

		Function()noexcept :mFunction(nullptr) {}

		Function(std::nullptr_t)noexcept :mFunction(nullptr) {}

		Function(function_t _function) :mFunction(_function) {}

		Function(const Function&) = default;

		Function& operator=(const Function&) = default;

		Function(Function&&) = default;

		Function& operator=(Function&&) = default;

		Function& operator=(std::nullptr_t) noexcept
		{
			mFunction = nullptr;
			return *this;
		}

		void operator()(ClassType* ptr, Args...args) const
		{
			if (mFunction == nullptr)
			{
				throw BadFunctionCall();
			}
			(ptr->*mFunction)(std::forward<Args>(args)...);
		}

	private:

		function_t mFunction;

	};

	template<class ReturnType, class ...Args>
	Function<ReturnType(Args...)> MakeFunction(ReturnType(*function)(Args...))
	{
		return Function<ReturnType(Args...)>(function);
	}

	template<class ClassType, class ReturnType, class ...Args>
	Function<ReturnType(ClassType::*)(Args...)> MakeFunction(ReturnType(ClassType::* function)(Args...))
	{
		return Function<ReturnType(ClassType::*)(Args...)>(function);
	}
}
