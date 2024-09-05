#include "stdafx.h"

namespace alg
{
	struct identity {
		using is_transparent = void;

		template<typename _Ty>
		constexpr _Ty&& operator()(_Ty&& _t) const noexcept {
			return static_cast<_Ty&&>(_t);
		}
	};

	template<typename _Ty, auto _Val>
	struct integral_constant {
		using type = _Ty;

		static constexpr _Ty value = _Val;
	};

	template<bool _Val>
	using bool_constant = integral_constant<bool, _Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	//Check whether type is addable
	template<typename _Ty>
	concept Addable = requires(_Ty v) { v + _Ty(); };

	//Check whether two types are same
	template<typename, typename>
	struct is_same : false_type { };

	template<typename _Ty>
	struct is_same<_Ty, _Ty> : true_type { };

	template<typename _Ty1, typename _Ty2>
	constexpr bool is_same_v = is_same<_Ty1, _Ty2>::value;

	//is type void?
	template<typename _Ty>
	constexpr bool is_void_v = is_same_v<_Ty, void>;

	//is type const?
	template<typename _Ty>
	struct is_const : false_type { };

	template<typename _Ty>
	struct is_const<const _Ty> : true_type { };

	template<typename _Ty>
	constexpr bool is_const_v = is_const<_Ty>::value;
	//FAILS WITH POINTERS OR REFERENCES

	template<typename _Ty>
	inline static constexpr bool is_rvalue_v = false;

	template<typename _Ty>
	inline static constexpr bool is_rvalue_v<_Ty&&> = true;

	template<typename _Ty>
	inline static constexpr bool is_lvalue_v = false;

	template<typename _Ty>
	inline static constexpr bool is_lvalue_v<_Ty&&> = true;

	template<typename _Ty>
	inline static constexpr bool is_reference_v = false;

	template<typename _Ty>
	inline static constexpr bool is_reference_v<_Ty&> = true;

	template<typename _Ty>
	inline static constexpr bool is_reference_v<_Ty&&> = true;

	template<typename _Ty>
	inline static constexpr bool is_pointer_v = false;

	template<typename _Ty>
	inline static constexpr bool is_pointer_v<_Ty*> = true;

	template<typename _Ty>
	inline static constexpr bool is_pointer_v<_Ty const*> = true;

	template<typename _Ty>
	struct is_reference : bool_constant<is_reference_v<_Ty>> {};

	template<typename _Ty>
	inline static constexpr bool is_function_v =
		!is_const_v<_Ty> && !is_reference_v<_Ty>;

	template<typename _Ty>
	struct is_function : bool_constant<is_function_v<_Ty>> {};

	template<bool _Test, typename _Ty>
	struct enable_if { };

	template<typename _Ty>
	struct enable_if<true, _Ty> {
		using type = _Ty;
	};

	template<bool _Test, typename _Ty>
	using enable_if_t = enable_if<_Test, _Ty>::type;

	template<bool _Test, typename _Ty1, typename _Ty2>
	struct conditionals {
		using type = _Ty1;
	};

	template<typename _Ty1, typename _Ty2>
	struct conditionals<false, _Ty1, _Ty2> {
		using type = _Ty2;
	};

	template<bool _Test, typename _Ty1, typename _Ty2>
	using conditionals_t = conditionals<_Test, _Ty1, _Ty2>::type;

	struct equal_to {
		template<class _Ty1, class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
			noexcept(noexcept(std::forward<_Ty1>(_Left) == std::forward<_Ty2>(_Right)))
		-> decltype(std::forward<_Ty1>(_Left) == std::forward<_Ty2>(_Right)) {
			return std::forward<_Ty1>(_Left) == std::forward<_Ty2>(_Right);
		}
	};

	//_Ty is not an object if it's a void
	template<typename _Ty>
	concept Object = !alg::is_void_v<_Ty>;
	//!!!!VOID CAN BE A POINTER OR REFERENCE!!

	template<typename _From, typename _To>
	concept convertible_to = requires(_From _obj) {
		static_cast<_To>(_obj);
	};

	//////////Iterator concepts//////////

	template<typename _It>
	concept iterator_readable = requires(_It it) {
		{ *it } -> Object;
	};

	template<typename _It, typename _Val>
	concept iterator_writable = requires(_It it, _Val _val) {
		{ *it = _val } -> alg::convertible_to<_Val>;
	};

	template<typename _It>
	concept iterator_incrementable = requires(_It it) {
		{ ++it } -> convertible_to<_It>;
		{ it++ } -> convertible_to<_It>;
	};

	//type is an iterator when one is incrementable and dereferenceable
	template<typename _It>
	concept _Input_Iterator = iterator_incrementable<_It> &&
		iterator_readable<_It>;

	template<typename _It, typename _Val>
	concept _Output_Iterator = iterator_incrementable<_It> &&
		iterator_writable<_It, _Val>;

	//_Ty is a function type when it has operator '()'
	template<typename _Ty>
	concept FunctionType = is_function_v<_Ty>;

	template<typename _Ty>
	struct remove_reference {
		using type = _Ty;
	};

	template<typename _Ty>
	struct remove_reference<_Ty&> {
		using type = _Ty;
	};

	template<typename _Ty>
	struct remove_reference<_Ty&&> {
		using type = _Ty;
	};

	template<typename _Ty>
	using remove_reference_t = typename remove_reference<_Ty>::type;
}