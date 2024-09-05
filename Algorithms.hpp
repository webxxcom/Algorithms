#pragma once
#include "stdafx.h"
#include "TypeTraits.hpp"

namespace alg
{
	template<typename _Ty>
	constexpr remove_reference_t<_Ty>&& move(_Ty&& _Val) noexcept {
		return static_cast<remove_reference_t<_Ty>&&>(_Val);
	}

	template<_Input_Iterator _Ty>
	constexpr void _is_valid(_Ty const _l, _Ty const _r) noexcept
	{
		if constexpr(alg::is_pointer_v<_Ty>){
			static_assert(_l <= _r, "The iterators should be valid to traverse through");
		} else {
			if (_l > _r) {
				std::abort();
			}
		}
	}

	template<_Input_Iterator InIt, FunctionType _Foo>
	constexpr InIt _Find_Unchecked_If(InIt _First, const InIt _Last, _Foo _Pred)
	{
		//_FIRST CAN BE PASSED AS CONST
		for (; _First != _Last; ++_First)
			if (_Pred(*_First))
				break;

		return _First;
	}
	
	template<_Input_Iterator InIt, Object _Val>
	constexpr InIt _Find_Unchecked(InIt _First, const InIt _Last, _Val const& _val)
	{
		//_FIRST CAN BE PASSED AS CONST
		for (; _First != _Last; ++_First)
			if (*_First == _val)
				break;

		return _First;
	}

	template<_Input_Iterator InIt, Object _Val>
	constexpr bool _Contains_Unchecked(InIt _First, InIt const _Last, _Val _val)
	{
		for (; _First != _Last; ++_First)
			if (*_First == _val)
				return true;

		return false;
	}

	template<Object _Val, _Output_Iterator<_Val> OutIt>
	constexpr OutIt _Replace_Unchecked(OutIt _First, OutIt _Last, _Val _Old_Value, _Val _New_Value)
	{
		for (; _First != _Last; ++_First)
			if (*_First == _Old_Value)
				*_First = _New_Value;

		return _First;
	}

	template<Object _Val, _Output_Iterator<_Val> OutIt, FunctionType _Foo>
	constexpr OutIt _Replace_Unchecked_If(OutIt _First, OutIt _Last, _Foo _Pred, _Val _New_Value)
	{
		for (; _First != _Last; ++_First)
			if (_Pred(*_First))
				*_First = _New_Value;

		return _First;
	}

	template<_Input_Iterator InIt, Object _Val>
	constexpr InIt find(InIt _First, const InIt _Last, _Val const& _val)
	{
		_is_valid(_First, _Last);

		return _Find_Unchecked(_First, _Last, _val);
	}

	template<_Input_Iterator InIt, Object _Val>
	constexpr InIt find_last(InIt _First, InIt const _Last, _Val const _val)
	{
		_is_valid(_First, _Last);

		InIt _last_found = _Last;
		for (; _First != _Last; ++_First)
			if (*_First == _val)
				_last_found = _First;

		return _last_found;
	}

	template<_Input_Iterator InIt, typename _Foo>
	constexpr InIt find_if(InIt _First, InIt const _Last, _Foo _Pred)
	{
		_is_valid(_First, _Last);

		return _Find_Unchecked_If(_First, _Last, _Pred);
	}

	template<_Input_Iterator InIt, typename _Foo>
	constexpr InIt find_if_not(InIt _First, InIt const _Last, _Foo _Pred)
	{
		_is_valid(_First, _Last);

		for (; _First != _Last; ++_First)
			if (!_Pred(*_First))
				break;

		return _First;
	}

	template<_Input_Iterator InIt1, _Input_Iterator InIt2>
	constexpr InIt1 find_first_of(InIt1 _First1, InIt1 const _Last1, InIt2 const _First2, InIt2 const _Last2)
	{
		_is_valid(_First1, _Last1);
		_is_valid(_First2, _Last2);

		for (; _First1 != _Last1; ++_First1)
			if (_Contains_Unchecked(_First2, _Last2, *_First1))
				break;

		return _First1;
	}

	template<_Input_Iterator InIt, Object _Val>
	constexpr bool contains(InIt _First, InIt const _Last, _Val _val)
	{
		_is_valid(_First, _Last);

		return _Contains_Unchecked(_First, _Last, _Val);
	}

	template<Object _Val, _Output_Iterator<_Val> OutIt>
	constexpr OutIt replace(OutIt _First, OutIt const _Last, _Val const& _Old_Value, _Val const& _New_Value)
	{
		_is_valid(_First, _Last);

		return _Replace_Unchecked(_First, _Last, _Old_Value, _New_Value);
	}

	template<Object _Val, _Output_Iterator<_Val> OutIt, FunctionType _Foo>
	constexpr OutIt replace_if(OutIt _First, OutIt const _Last, _Foo _Pred, _Val const& _New_Value)
	{
		_is_valid(_First, _Last);

		return _Replace_Unchecked_If(_First, _Last, _Pred, _New_Value);
	}

	template<_Input_Iterator InIt1, _Input_Iterator InIt2>
	constexpr bool equal(InIt1 _First1, InIt1 _Last1, InIt2 _First2, InIt2 _Last2)
	{
		_is_valid(_First1, _Last1);
		_is_valid(_First2, _Last2);

		bool _result = true;
		for (; _First1 != _Last1; ++_First1, ++_First2)
			if (*_First1 != *_First2)
				_result = false;

		if (_First2 != _Last2)
			_result = false;

		return _result;
	}

	template<_Input_Iterator InIt1, _Input_Iterator InIt2>
	constexpr bool equal(InIt1 _First1, InIt1 _Last1, InIt2 _First2)
	{
		_is_valid(_First1, _Last1);

		bool _result = true;

		for (; _First1 != _Last1; ++_First1, ++_First2)
			if (*_First1 != *_First2)
				_result = false;

		return _result;
	}

	template<_Input_Iterator InIt, _Output_Iterator<typename InIt::value_type> OutIt>
	constexpr void move(InIt _First, InIt const _Last, OutIt _DFirst)
	{
		_is_valid(_First, _Last);

		for (; _First != _Last; ++_First, ++_DFirst)
			*_DFirst = alg::move(*_First);
	}

	template<_Input_Iterator InIt, typename _Val>
	constexpr _Val accumulate(InIt _First, InIt const _Last, _Val _val = _Val())
	{
		_is_valid(_First, _Last);

		_Val _sum = _val;
		for (; _First != _Last; ++_First)
			_sum = _sum + *_First;

		return _sum;
	}

	template<_Input_Iterator InIt, FunctionType _Pred>
	constexpr InIt adjacent_find(InIt _First, InIt const _Last, _Pred _pred)
	{
		_is_valid(_First, _Last);

		InIt next = _First;
		++_First;

		for (; _First != _Last; ++_First, ++next)
			if (_pred(*next, *_First))
				break;

		return _First;
	}

	template<_Input_Iterator InIt>
	constexpr InIt adjacent_find(InIt _First, InIt const _Last)
	{
		return adjacent_find(_First, _Last, alg::equal_to());
	}
}