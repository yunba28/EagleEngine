#pragma once

#include <memory>
#include <Concept/Invocable.hpp>

namespace eagle
{

	class BadCallFunction : public std::exception
	{
	public:
		BadCallFunction() :exception("Bad call function") {}
		BadCallFunction(const char* const inMessage) :exception(inMessage) {}
	};

	template<class>
	class Function;

	template<class ReturnType, class ...Args>
	class Function<ReturnType(Args...)>
	{
	private:

		class FuncBase
		{
		public:
			virtual ReturnType invoke(Args&&...) = 0;
		};

		template<class Functor>
		class FuncImpl : public FuncBase
		{
		public:
			FuncImpl(Functor newFunctor)
				: functor(newFunctor)
			{}
			ReturnType invoke(Args&& ...inArgs)override
			{
				return functor(std::forward<Args>(inArgs)...);
			}
			Functor functor;
		};

	public:

		using function_t = ReturnType(*)(Args...);

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		template<class Functor>
		Function(Functor newFunctor)requires(Concept::Invocable<Functor, ReturnType, Args...>)
			: mFunc(std::make_shared<FuncImpl<Functor>>(newFunctor))
		{}

		template<class Functor>
		Function& operator=(Functor newFunctor)requires(Concept::Invocable<Functor, ReturnType, Args...>)
		{
			mFunc = std::make_shared<FuncImpl<Functor>>(newFunctor);
			return *this;
		}

	public:

		ReturnType operator()(Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			return mFunc->invoke(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		std::shared_ptr<FuncBase> mFunc = nullptr;

	};

	template<class ...Args>
	class Function<void(Args...)>
	{
	private:

		class FuncBase
		{
		public:
			virtual void invoke(Args&&...) = 0;
		};

		template<class Functor>
		class FuncImpl : public FuncBase
		{
		public:
			FuncImpl(Functor newFunctor)
				: functor(newFunctor)
			{}
			void invoke(Args&& ...inArgs)override
			{
				functor(std::forward<Args>(inArgs)...);
			}
			Functor functor;
		};

	public:

		using function_t = void(*)(Args...);

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		template<class Functor>
		Function(Functor newFunctor)requires(Concept::Invocable<Functor, void, Args...>)
			: mFunc(std::make_shared<FuncImpl<Functor>>(newFunctor))
		{}

		template<class Functor>
		Function& operator=(Functor newFunctor)requires(Concept::Invocable<Functor, void, Args...>)
		{
			mFunc = std::make_shared<FuncImpl<Functor>>(newFunctor);
			return *this;
		}

	public:

		void operator()(Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			mFunc->invoke(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		std::shared_ptr<FuncBase> mFunc = nullptr;

	};

	template<class ClassType, class ReturnType, class ...Args>
	class Function<ReturnType(ClassType::*)(Args...)>
	{
	public:

		using function_t = ReturnType(ClassType::*)(Args...);

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		Function(function_t newFunctor)
			: mFunc(newFunctor)
		{}

		Function& operator=(function_t newFunctor)
		{
			mFunc = newFunctor;
			return *this;
		}

	public:

		ReturnType operator()(ClassType& inUser, Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			return (inUser.*mFunc)(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		function_t mFunc = nullptr;

	};

	template<class ClassType, class ReturnType, class ...Args>
	class Function<ReturnType(ClassType::*)(Args...) const>
	{
	public:

		using function_t = ReturnType(ClassType::*)(Args...);

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		Function(function_t newFunctor)
			: mFunc(newFunctor)
		{}

		Function& operator=(function_t newFunctor)
		{
			mFunc = newFunctor;
			return *this;
		}

	public:

		ReturnType operator()(const ClassType& inUser, Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			return (inUser.*mFunc)(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		function_t mFunc = nullptr;

	};

	template<class ClassType, class ...Args>
	class Function<void(ClassType::*)(Args...)>
	{
	public:

		using function_t = void(ClassType::*)(Args...)const;

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		Function(function_t newFunctor)
			: mFunc(newFunctor)
		{}

		Function& operator=(function_t newFunctor)
		{
			mFunc = newFunctor;
			return *this;
		}

	public:

		void operator()(ClassType& inUser, Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			(inUser.*mFunc)(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		function_t mFunc = nullptr;

	};

	template<class ClassType, class ...Args>
	class Function<void(ClassType::*)(Args...) const>
	{
	public:

		using function_t = void(ClassType::*)(Args...)const;

	public:

		Function() = default;
		~Function() = default;

		Function(Function&&) = default;
		Function(const Function&) = default;
		Function& operator=(Function&&) = default;
		Function& operator=(const Function&) = default;

		Function(std::nullptr_t)noexcept
			: mFunc(nullptr)
		{}

		Function& operator=(std::nullptr_t)noexcept
		{
			mFunc = nullptr;
			return *this;
		}

		Function(function_t newFunctor)
			: mFunc(newFunctor)
		{}

		Function& operator=(function_t newFunctor)
		{
			mFunc = newFunctor;
			return *this;
		}

	public:

		void operator()(const ClassType& inUser, Args&& ...inArgs)const
		{
			if (mFunc == nullptr)
			{
				throw BadCallFunction{};
			}
			(inUser.*mFunc)(std::forward<Args>(inArgs)...);
		}

		explicit operator bool()const noexcept
		{
			return mFunc != nullptr;
		}

	private:

		function_t mFunc = nullptr;

	};
}
