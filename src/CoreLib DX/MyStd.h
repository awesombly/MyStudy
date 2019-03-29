#pragma once
#ifndef MYSTD
#define MYSTD

// ��Ÿ�� Ÿ�� �Լ���
namespace myStd {
	// 32��Ʈ�� sqrt
	//double inline __declspec (naked) __fastcall sqrt14(double n)
	//{
	//	_asm fld qword ptr[esp + 4]
	//	_asm fsqrt
	//	_asm ret 8
	//}

	// �ణ ���� sqrt
	//float sqrtQuick(const float& value) noexcept
	//{
	//	union
	//	{
	//		int i;
	//		float x;
	//	} u;
	//	u.x = value;
	//	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
	//	// Two Babylonian Steps (simplified from:)
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	// u.x = 0.5f * (u.x + x/u.x);
	//	u.x = u.x + value / u.x;
	//	u.x = 0.25f * u.x + value / u.x;
	//	return u.x;
	//}

	//  ���ø� abs
	template <typename Type>
	constexpr Type AbsT(const Type& value) noexcept
	{
		return value > 0 ? value : -value;
	}
	// ���ø� pow
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type& result, const Type2& value) noexcept
	{
		return value <= 1 ? value : PowT(target, result * target, value - 1);
	}
	template <typename Type, typename Type2>
	constexpr Type PowT(const Type& target, const Type2& value) noexcept
	{
		return value <= 1 ? target : PowT(target, target * target, value - 1);
	}
	template <typename Type>
	constexpr Type PowT(const Type& target) noexcept
	{
		return target * target;
	}
	/*template <typename Type>
	struct PowT
	{
		enum { value = target * target }
	};*/

	// ������Ÿ�� sin
	constexpr float SinSeries(const float& radian, const int& cnt, const int& maxTerms) noexcept
	{
		return cnt > maxTerms ? 1.0f : 
			1.0f - (radian * radian / (2.0f * cnt + 2.0f) / (2.0f * cnt + 3.0f) * SinSeries(radian, cnt + 1, maxTerms));
	}
	constexpr float Sin(const float& radian) noexcept
	{
		constexpr int maxTerms = 10;
		return radian * SinSeries(radian, 0, maxTerms);
	}
	
	// con, tan, 
}

#endif // MYSTD