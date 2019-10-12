#pragma once

#include <memory>
#include <type_traits>
#include <string>


template<class T>
class DeepPtr: private std::unique_ptr<T> {
public:
	static_assert(!std::is_polymorphic_v<T>, "Polymorphic type not allowed!");
	using PtrType = std::unique_ptr<T>;
	// normal constructor
	DeepPtr(T* ptr = nullptr) : PtrType{ ptr } {}
	// copy 
	DeepPtr(DeepPtr const& other) {
		*this = other;
	}
	DeepPtr& operator=(DeepPtr const& other) {
		if (other)
			static_cast<PtrType&>(*this) = std::make_unique<T>(*other);
		else
			reset();
		return *this;
	}
	// move
	DeepPtr(DeepPtr&& other) noexcept {
		*this = std::move(other);
	}
	DeepPtr& operator=(DeepPtr&& other) noexcept {
		static_cast<PtrType&>(*this) = std::move(other);
		return *this;
	}
	// access
	using PtrType::operator*;
	using PtrType::operator->;
	using PtrType::get;
	using PtrType::operator bool;
	// modify
	using PtrType::reset;
	using PtrType::release;
	using PtrType::swap;
	// make unique
	template<class... Args>
	static DeepPtr make_unique(Args&&... args)
	{
		DeepPtr ptr{};
		static_cast<PtrType&>(ptr) =
			std::make_unique<T>(std::forward<Args>(args)...);
		return ptr;
	}
};
