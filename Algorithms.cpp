#pragma once
#include "stdafx.h"
#include "Algorithms.h"

#define _ALG alg::

template<typename _Ty>
_ALG remove_reference_t<_Ty>&& _ALG move(_Ty&& _Val) {
    return static_cast<remove_reference_t<_Ty>&&>(_Val);
}

/// <summary>
/// Checks if _l and _r are iterators in valid range(_l <= _r)
/// </summary>
template<_ALG _Input_Iterator _Ty>
void _ALG _is_valid(_Ty const _l, _Ty const _r)
{
    if (_l > _r)
        throw std::runtime_error("The iterators should be valid to traverse through");
}

template<_ALG _Input_Iterator InIt, _ALG FunctionType _Foo>
InIt _ALG _Find_Unchecked_If(InIt _First, const InIt _Last, _Foo _Pred)
{
    //_FIRST CAN BE PASSED AS CONST
    for (; _First != _Last; ++_First)
        if (_Pred(*_First))
            break;

    return _First;
}

template<_ALG _Input_Iterator InIt, _ALG Object _Val>
InIt _ALG _Find_Unchecked(InIt _First, const InIt _Last, _Val const& _val)
{
    //_FIRST CAN BE PASSED AS CONST
    for (; _First != _Last; ++_First)
        if (*_First == _val)
            break;

    return _First;
}

template<_ALG _Input_Iterator InIt, _ALG Object Value>
bool _ALG _Contains_Unchecked(InIt _First, InIt const _Last, Value _Val)
{
    for (; _First != _Last; ++_First)
        if (*_First == _Val)
            return true;

    return false;
}

template<_ALG Object _Val, _ALG _Output_Iterator<_Val> OutIt>
OutIt _ALG _Replace_Unchecked(OutIt _First, OutIt _Last, _Val _Old_Value, _Val _New_Value)
{
    for (; _First != _Last; ++_First)
        if (*_First == _Old_Value)
            *_First = _New_Value;

    return _First;
}

template<_ALG Object _Val, _ALG _Output_Iterator<_Val> OutIt, _ALG FunctionType _Foo>
OutIt _ALG _Replace_Unchecked_If(OutIt _First, OutIt _Last, _Foo _Pred, _Val _New_Value)
{
    for (; _First != _Last; ++_First)
        if (_Pred(*_First))
            *_First = _New_Value;

    return _First;
}

/// <summary>
/// Finds the first occurrence of _val in the range [_First; _Last)
/// </summary>
template<_ALG _Input_Iterator InIt, _ALG Object _Val>
InIt _ALG find(InIt _First, InIt const _Last, _Val const& _val)
{
    _is_valid(_First, _Last);

    return _Find_Unchecked(_First, _Last, _val);
}

/// <summary>
/// Finds the last occurrence of _val in the range [_First; _Last)
/// </summary>
/// <typeparam name="_Ty"></typeparam>
/// <typeparam name="_Val"></typeparam>
/// <param name="_First"> - must be copyable</param>
template<_ALG _Input_Iterator InIt, _ALG Object _Val>
InIt _ALG find_last(InIt _First, InIt const _Last, _Val const _val)
{
    _ALG _is_valid(_First, _Last);

    auto _last_found = _Last;
    for (; _First != _Last; ++_First)
        if (*_First == _val)
            _last_found = _First;

    return _last_found;
}

/// <summary>
/// Returns the first occurence of true of _Pred in [_First, _Last)
/// </summary>
/// <typeparam name="_Foo"></typeparam>
/// <typeparam name="InIt"></typeparam>
/// <param name="_Pred"> - is a boolean function that takes one parameter</param>
/// <returns></returns>
template<_ALG _Input_Iterator InIt, typename _Foo>
InIt _ALG find_if(InIt _First, InIt const _Last, _Foo _Pred)
{
    _ALG _is_valid(_First, _Last);

    return _ALG _Find_Unchecked_If(_First, _Last, _Pred);
}

/// <summary>
/// Returns the first occurence of true of _Pred in [_First, _Last)
/// </summary>
/// <typeparam name="_Foo"></typeparam>
/// <typeparam name="InIt"></typeparam>
/// <param name="_Pred"> - is a boolean function that takes one parameter</param>
/// <returns></returns>
template<_ALG _Input_Iterator InIt, typename _Foo>
InIt _ALG find_if_not(InIt _First, InIt const _Last, _Foo _Pred)
{
    _ALG _is_valid(_First, _Last);

    for (; _First != _Last; ++_First)
        if (!_Pred(*_First))
            break;

    return _First;
}

template<_ALG _Input_Iterator InIt1, _ALG _Input_Iterator InIt2>
InIt1 _ALG find_first_of(InIt1 _First1, InIt1 const _Last1, InIt2 const _First2, InIt2 const _Last2)
{
    _ALG _is_valid(_First1, _Last1);
    _ALG _is_valid(_First2, _Last2);

    for (; _First1 != _Last1; ++_First1)
        if (_ALG _Contains_Unchecked(_First2, _Last2, *_First1))
            break;

    return _First1;
}

template<_ALG _Input_Iterator InIt, _ALG Object Value>
bool _ALG contains(InIt _First, InIt const _Last, Value _Val)
{
    _ALG _is_valid(_First, _Last);

    return _ALG _Contains_Unchecked(_First, _Last, _Val);
}

template<_ALG Object _Val, _ALG _Output_Iterator<_Val> OutIt>
OutIt _ALG replace(OutIt _First, OutIt const _Last, _Val const& _Old_Value, _Val const& _New_Value)
{
    _ALG _is_valid(_First, _Last);

    return _ALG _Replace_Unchecked(_First, _Last, _Old_Value, _New_Value);
}

template<_ALG Object _Val, _ALG _Output_Iterator<_Val> OutIt, _ALG FunctionType _Foo>
OutIt _ALG replace_if(OutIt _First, OutIt const _Last, _Foo _Pred, _Val const& _New_Value)
{
    _ALG _is_valid(_First, _Last);

    return _ALG _Replace_Unchecked_If(_First, _Last, _Pred, _New_Value);
}

template<_ALG _Input_Iterator InIt1, _ALG _Input_Iterator InIt2>
bool _ALG equal(InIt1 _First1, InIt1 _Last1, InIt2 _First2, InIt2 _Last2)
{
    _ALG _is_valid(_First1, _Last1);
    _ALG _is_valid(_First2, _Last2);

    bool _result = true;
    for (; _First1 != _Last1; ++_First1, ++_First2)
        if (*_First1 != *_First2)
            _result = false;

    if (_First2 != _Last2)
        _result = false;

    return _result;
}

template<_ALG _Input_Iterator InIt1, _ALG _Input_Iterator InIt2>
bool _ALG equal(InIt1 _First1, InIt1 _Last1, InIt2 _First2)
{
    _ALG _is_valid(_First1, _Last1);

    bool _result = true;

    for (; _First1 != _Last1; ++_First1, ++_First2)
        if (*_First1 != *_First2)
            _result = false;

    return _result;
}

template<_ALG _Input_Iterator InIt, _ALG _Output_Iterator<typename InIt::value_type> OutIt>
void _ALG move(InIt _First, InIt const _Last, OutIt _DFirst)
{
    _is_valid(_First, _Last);

    for (; _First != _Last; ++_First, ++_DFirst)
        *_DFirst = _ALG move(*_First);
}

/// <summary>
/// Sums up the values in the range [_First, _Last) to _val
/// </summary>
/// <typeparam name="_Ty"></typeparam>
/// <typeparam name="_Val"></typeparam>
/// <param name="_val"> - must be addable and assignable</param>
/// <returns></returns>
template<_ALG _Input_Iterator InIt, typename _Val>
_Val _ALG accumulate(InIt _First, InIt const _Last, _Val _val)
{
    _ALG _is_valid(_First, _Last);

    _Val _sum = _val;
    for (; _First != _Last; ++_First)
        _sum = _sum + *_First;

    return _sum;
}
