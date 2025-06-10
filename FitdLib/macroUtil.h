#ifndef _MACRO_UTIL_H_
#define _MACRO_UTIL_H_

#ifndef FORCEINLINE
// TODO: The commented section is taken from "endianess.h", check if better than pre-existing.
/* #ifdef __GCC__
#define FORCEINLINE static inline
#else
#ifdef WIN32
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE static inline
#endif
#endif */

#ifdef UNIX
#define FORCEINLINE static inline
#else
#ifdef WIN32
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif
#endif
#endif
// Define bitwise operators for an enum class, allowing usage as bitmasks. https://voithos.io/articles/enum-class-bitmasks/
#define _ENUM_CLASS_FLAGS(Enum, FORCEINLINE)						\
/* #region + */														\
FORCEINLINE constexpr Enum operator+(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) +			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator+=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) +	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator+(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) +			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator+=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) +	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion + */													\
/* #region - */														\
FORCEINLINE constexpr Enum operator-(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) -			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator-=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) -	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator-(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) -			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator-=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) -	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion - */													\
/* #region * */														\
FORCEINLINE constexpr Enum operator*(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) *			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator*=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) *	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator*(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) *			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator*=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) *	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion * */													\
/* #region / */														\
FORCEINLINE constexpr Enum operator/(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) /			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator/=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) /	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator/(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) /			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator/=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) /	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion / */													\
/* #region Bitwise Or */											\
FORCEINLINE constexpr Enum operator|(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) |			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator|=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) |	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator|(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) |			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator|=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) |	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion Bitwise Or */											\
/* #region Bitwise And */											\
FORCEINLINE constexpr Enum operator&(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) &			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator&=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) &	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator&(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) &			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator&=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) &	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion Bitwise And */										\
/* #region Bitwise Xor */											\
FORCEINLINE constexpr Enum operator^(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) ^			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator^=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) ^	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator^(Enum Lhs, Enum Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) ^			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator^=(Enum& Lhs, Enum Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) ^	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion Bitwise Xor */										\
/* #region Bit Shifts */											\
FORCEINLINE constexpr Enum operator<<(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) <<			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator<<=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) <<	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
FORCEINLINE constexpr Enum operator>>(Enum Lhs, int Rhs) {			\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) >>			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& operator>>=(Enum& Lhs, int Rhs) {					\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) >>	\
				static_cast<std::underlying_type_t<Enum>>(Rhs));	\
}																	\
/* #endregion Bit Shifts */											\
/* #region < */														\
FORCEINLINE constexpr bool operator<(Enum Lhs, int Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) <			\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator<=(Enum Lhs, int Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) <=		\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator<(Enum Lhs, Enum Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) <			\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator<=(Enum Lhs, Enum Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) <=		\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
/* #endregion < */													\
/* #region > */														\
FORCEINLINE constexpr bool operator>(Enum Lhs, int Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) >			\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator>=(Enum Lhs, int Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) >=		\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator>(Enum Lhs, Enum Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) >			\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
FORCEINLINE constexpr bool operator>=(Enum Lhs, Enum Rhs) {			\
	return static_cast<std::underlying_type_t<Enum>>(Lhs) >=		\
		static_cast<std::underlying_type_t<Enum>>(Rhs);				\
}																	\
/* #endregion > */													\
FORCEINLINE constexpr Enum operator~(Enum E) {						\
		return static_cast<Enum>(									\
			~static_cast<std::underlying_type_t<Enum>>(E));			\
}
#define ENUM_CLASS_FLAGS(Enum) _ENUM_CLASS_FLAGS(Enum, FORCEINLINE)
/* #define _symPlus +
#define __symPlus _symPlus
#define _symMinus -
#define __symMinus _symMinus
#define _symMulp *
#define __symMulp _symMulp
#define _symDiv \/
#define __symDiv _symDiv
#define _symBOr \|
#define __symBOr _symBOr
#define _symBAnd &
#define __symBAnd _symBAnd
#define _symBXor \^
#define __symBXor _symBXor
#define _num9 operator
#define __operator(op) op
#define _operator(op) operator##__operator(op)
#define _ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, op)			\
FORCEINLINE constexpr Enum _operator(op)(Enum Lhs, int Rhs) {		\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) op			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& _operator(op)=(Enum& Lhs, int Rhs) {			\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) op	\
				static_cast<std::underlying_type_t<Enum>>(Lhs));	\
}																	\
FORCEINLINE constexpr Enum _operator(op)(Enum Lhs, Enum Rhs) {		\
	return static_cast<Enum>(										\
		static_cast<std::underlying_type_t<Enum>>(Lhs) op			\
		static_cast<std::underlying_type_t<Enum>>(Rhs));			\
}																	\
FORCEINLINE Enum& _operator(op)=(Enum& Lhs, Enum Rhs) {			\
	return Lhs = static_cast<Enum>(									\
				static_cast<std::underlying_type_t<Enum>>(Lhs) op	\
				static_cast<std::underlying_type_t<Enum>>(Lhs));	\
}
#define ENUM_CLASS_OVERLOAD_BINARY(Enum, op) _ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, op)
#define _ENUM_CLASS_FLAGS(Enum, FORCEINLINE)							\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symPlus)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symMinus)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symMulp)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symDiv)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symBOr)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symBAnd)					\
	_ENUM_CLASS_OVERLOAD_BINARY(Enum, FORCEINLINE, __symBXor)					\
	FORCEINLINE constexpr Enum operator~(Enum E) {						\
		return static_cast<Enum>(										\
			~static_cast<std::underlying_type_t<Enum>>(E));				\
	}
#define ENUM_CLASS_FLAGS(Enum) _ENUM_CLASS_FLAGS(Enum, FORCEINLINE) */
#endif
