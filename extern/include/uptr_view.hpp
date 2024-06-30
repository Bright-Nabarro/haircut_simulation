#pragma once
#include <memory>
#include <utility>

namespace tiny_utl
{

template <typename UP, typename Ty>
class uptr_view_base
{
public:
	// constructor
	uptr_view_base() noexcept : mp_uptr{nullptr}
	{ }

	uptr_view_base(std::nullptr_t) noexcept : mp_uptr(nullptr)
	{ }

	uptr_view_base(UP& uptr) noexcept : mp_uptr{&uptr}
	{ }

	uptr_view_base& operator=(UP& ptr) noexcept
	{
		mp_uptr = &ptr;
		return *this;
	}

	uptr_view_base(UP&&) noexcept = delete;

	uptr_view_base& operator=(UP&&) noexcept = delete;

	// big five use default
	virtual ~uptr_view_base() = default;

	bool operator==(const uptr_view_base& rhs) const noexcept
	{
		return mp_uptr == rhs.mp_uptr;
	}

	bool operator!=(const uptr_view_base& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	bool operator==(std::nullptr_t) const noexcept
	{
		return mp_uptr == nullptr || *mp_uptr == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept
	{
		return !(*this == nullptr);
	}

	bool is_reference(const UP& uptr)
	{
		return mp_uptr == &uptr;
	}

	explicit operator bool () const noexcept
	{
		return this->operator!=(nullptr);
	}

	const Ty& operator*() const noexcept
	{
		return **mp_uptr;
	}

	const Ty* operator->() const noexcept
	{
		return mp_uptr->get();
	}

	const Ty* get() const noexcept
	{
		return mp_uptr->get();
	}

	Ty& operator*() noexcept
	{
		return const_cast<Ty&>(std::as_const(*this).operator*());
	}

	Ty* operator->() noexcept
	{
		return const_cast<Ty*>(std::as_const(*this).operator->());
	}

	Ty* get() noexcept
	{
		return const_cast<Ty*>(std::as_const(*this).get());
	}

	UP& release() noexcept
	{
		auto* result = mp_uptr;
		mp_uptr = nullptr;
		return *result;
	}

protected:
	UP* mp_uptr;
};


// reference const uptr
template <typename Ty, typename Dp = std::default_delete<Ty>>
class cuptr_view : public uptr_view_base<const std::unique_ptr<Ty, Dp>, Ty>
{
public:
	using uptr_view_base<const std::unique_ptr<Ty, Dp>, Ty>::uptr_view_base;
	~cuptr_view() override = default;
};

// CTAD
template <typename Ty, typename Dp = std::default_delete<Ty>>
cuptr_view(const std::unique_ptr<Ty, Dp>) -> cuptr_view<Ty, Dp>;


// reference const uptr
template <typename Ty, typename Dp = std::default_delete<Ty>>
class uptr_view : public uptr_view_base<std::unique_ptr<Ty, Dp>, Ty>
{
public:
	using uptr_view_base<std::unique_ptr<Ty, Dp>, Ty>::uptr_view_base;
	~uptr_view() override = default;
};

// CTAD
template <typename Ty, typename Dp = std::default_delete<Ty>>
uptr_view(std::unique_ptr<Ty, Dp>) -> uptr_view<Ty, Dp>;



} // namespace tiny_utl
