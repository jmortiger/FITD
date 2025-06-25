#ifndef _POINTER_H_
#define _POINTER_H_
// #region Increment & Read
/* // template<typename T, typename U> T advPtr(U* &ptr) { return *(((T*)ptr)++); }
template<typename T, typename U> T advPtr(U* &ptr);
// template<typename U> u8 advPtr(U* &ptr) { u8 r = *(u8*)ptr; ptr = &((u8*)ptr)[1]; return r; }
template<> u8 advPtr(u8* &ptr) { u8 r = *(u8*)ptr; ptr = &((u8*)ptr)[1]; return r; }
template<> u8 advPtr(s8* &ptr) { u8 r = *(u8*)ptr; ptr = &((s8*)ptr)[1]; return r; }
// template<typename U> s8 advPtr(U* &ptr) { s8 r = *(s8*)ptr; ptr = &((s8*)ptr)[1]; return r; }
template<> s8 advPtr(u8* &ptr) { s8 r = *(s8*)ptr; ptr = &((u8*)ptr)[1]; return r; }
template<> s8 advPtr(s8* &ptr) { s8 r = *(s8*)ptr; ptr = &((s8*)ptr)[1]; return r; }
// template<typename U> u16 advPtr(U* &ptr) { u16 r = *(u16*)ptr; ptr = &((u16*)ptr)[1]; return r; }
template<> u16 advPtr(u8* &ptr) { u16 r = *(u16*)ptr; ptr = (u8*)(&((u16*)ptr)[1]); return r; }
template<> u16 advPtr(s8* &ptr) { u16 r = *(u16*)ptr; ptr = (s8*)(&((u16*)ptr)[1]); return r; }
// template<typename U> s16 advPtr(U* &ptr) { s16 r = *(s16*)ptr; ptr = &((s16*)ptr)[1]; return r; }
template<> s16 advPtr(u8* &ptr) { s16 r = *(s16*)ptr; ptr = &((s16*)ptr)[1]; return r; }
template<> s16 advPtr(s8* &ptr) { s16 r = *(s16*)ptr; ptr = &((s16*)ptr)[1]; return r; }
// template<typename U> u32 advPtr(U* &ptr) { u32 r = *(u32*)ptr; ptr = &((u32*)ptr)[1]; return r; }
template<> u32 advPtr(u8* &ptr) { u32 r = *(u32*)ptr; ptr = &((u32*)ptr)[1]; return r; }
template<> u32 advPtr(s8* &ptr) { u32 r = *(u32*)ptr; ptr = &((u32*)ptr)[1]; return r; }
// template<typename U> s32 advPtr(U* &ptr) { s32 r = *(s32*)ptr; ptr = &((s32*)ptr)[1]; return r; }
template<> s32 advPtr(u8* &ptr) { s32 r = *(s32*)ptr; ptr = &((s32*)ptr)[1]; return r; }
template<> s32 advPtr(s8* &ptr) { s32 r = *(s32*)ptr; ptr = &((s32*)ptr)[1]; return r; } */
class AutoAdvancePtr {
public:
	u8* ptr;
	AutoAdvancePtr(u8* ptr);
	void initialize(u8* ptr);
	s8 readS8();
	s8 readS8LE();
	s8 readS8BE();
	u8 readU8();
	u8 readU8LE();
	u8 readU8BE();
	s16 readS16();
	s16 readS16LE();
	s16 readS16BE();
	u16 readU16();
	u16 readU16LE();
	u16 readU16BE();
	s32 readS32();
	s32 readS32LE();
	s32 readS32BE();
	u32 readU32();
	u32 readU32LE();
	u32 readU32BE();
	s8 readOnlyS8();
	s8 readOnlyS8LE();
	s8 readOnlyS8BE();
	u8 readOnlyU8();
	u8 readOnlyU8LE();
	u8 readOnlyU8BE();
	s16 readOnlyS16();
	s16 readOnlyS16LE();
	s16 readOnlyS16BE();
	u16 readOnlyU16();
	u16 readOnlyU16LE();
	u16 readOnlyU16BE();
	s32 readOnlyS32();
	s32 readOnlyS32LE();
	s32 readOnlyS32BE();
	u32 readOnlyU32();
	u32 readOnlyU32LE();
	u32 readOnlyU32BE();
};// typedef class AutoAdvancePtr AutoAdvancePtr;
/* #define _readS8(ptr) ((s8)*(ptr++))
#define readS8(ptr) _readS8(ptr)
#define _readS8LE(ptr) ((s8)*(ptr++))
#define readS8LE(ptr) _readS8LE(ptr)
#define _readS8BE(ptr) ((s8)*(ptr++))
#define readS8BE(ptr) _readS8BE(ptr)
#define _readU8(ptr) ((u8)*(ptr++))
#define readU8(ptr) _readU8(ptr)
#define _readU8LE(ptr) ((u8)*(ptr++))
#define readU8LE(ptr) _readU8LE(ptr)
#define _readU8BE(ptr) ((u8)*(ptr++))
#define readU8BE(ptr) _readU8BE(ptr) */
/* #define readS8(ptr) ((s8)*(ptr++))
#define readS8LE(ptr) ((s8)*(ptr++))
#define readS8BE(ptr) ((s8)*(ptr++))
#define readU8(ptr) ((u8)*(ptr++))
#define readU8LE(ptr) ((u8)*(ptr++))
#define readU8BE(ptr) ((u8)*(ptr++)) */
s8 readS8(u8* &ptr, int* count = NULL);
s8 readS8(s8* &ptr, int* count = NULL);
s8 readS8(char* &ptr, int* count = NULL);
s8 readS8LE(u8* &ptr, int* count = NULL);
s8 readS8LE(s8* &ptr, int* count = NULL);
s8 readS8LE(char* &ptr, int* count = NULL);
s8 readS8BE(u8* &ptr, int* count = NULL);
s8 readS8BE(s8* &ptr, int* count = NULL);
s8 readS8BE(char* &ptr, int* count = NULL);
u8 readU8(u8* &ptr, int* count = NULL);
u8 readU8(s8* &ptr, int* count = NULL);
u8 readU8(char* &ptr, int* count = NULL);
u8 readU8LE(u8* &ptr, int* count = NULL);
u8 readU8LE(s8* &ptr, int* count = NULL);
u8 readU8LE(char* &ptr, int* count = NULL);
u8 readU8BE(u8* &ptr, int* count = NULL);
u8 readU8BE(s8* &ptr, int* count = NULL);
u8 readU8BE(char* &ptr, int* count = NULL);
s16 readS16(u8* &ptr, int* count = NULL);
s16 readS16(s8* &ptr, int* count = NULL);
s16 readS16(char* &ptr, int* count = NULL);
s16 readS16LE(u8* &ptr, int* count = NULL);
s16 readS16LE(s8* &ptr, int* count = NULL);
s16 readS16LE(char* &ptr, int* count = NULL);
s16 readS16BE(u8* &ptr, int* count = NULL);
s16 readS16BE(s8* &ptr, int* count = NULL);
s16 readS16BE(char* &ptr, int* count = NULL);
u16 readU16(u8* &ptr, int* count = NULL);
u16 readU16(s8* &ptr, int* count = NULL);
u16 readU16(char* &ptr, int* count = NULL);
u16 readU16LE(u8* &ptr, int* count = NULL);
u16 readU16LE(s8* &ptr, int* count = NULL);
u16 readU16LE(char* &ptr, int* count = NULL);
u16 readU16BE(u8* &ptr, int* count = NULL);
u16 readU16BE(s8* &ptr, int* count = NULL);
u16 readU16BE(char* &ptr, int* count = NULL);
s32 readS32(u8* &ptr, int* count = NULL);
s32 readS32(s8* &ptr, int* count = NULL);
s32 readS32(char* &ptr, int* count = NULL);
s32 readS32LE(u8* &ptr, int* count = NULL);
s32 readS32LE(s8* &ptr, int* count = NULL);
s32 readS32LE(char* &ptr, int* count = NULL);
s32 readS32BE(u8* &ptr, int* count = NULL);
s32 readS32BE(s8* &ptr, int* count = NULL);
s32 readS32BE(char* &ptr, int* count = NULL);
u32 readU32(u8* &ptr, int* count = NULL);
u32 readU32(s8* &ptr, int* count = NULL);
u32 readU32(char* &ptr, int* count = NULL);
u32 readU32LE(u8* &ptr, int* count = NULL);
u32 readU32LE(s8* &ptr, int* count = NULL);
u32 readU32LE(char* &ptr, int* count = NULL);
u32 readU32BE(u8* &ptr, int* count = NULL);
u32 readU32BE(s8* &ptr, int* count = NULL);
u32 readU32BE(char* &ptr, int* count = NULL);
// #region void*
/* s16 readS16(void* &ptr) {
	s16 r = *(s16*)ptr;
	ptr += 2;
	return r;
}

s16 readS16LE(void* &ptr) {
	s16 r = READ_LE_S16(ptr);
	ptr += 2;
	return r;
}

s16 readS16BE(void* &ptr) {
	s16 r = READ_BE_S16(ptr);
	ptr += 2;
	return r;
}

u16 readU16(void* &ptr) {
	u16 r = *(u16*)ptr;
	ptr += 2;
	return r;
}

u16 readU16LE(void* &ptr) {
	u16 r = READ_LE_U16(ptr);
	ptr += 2;
	return r;
}

u16 readU16BE(void* &ptr) {
	u16 r = READ_BE_U16(ptr);
	ptr += 2;
	return r;
}

s32 readS32(void* &ptr) {
	s32 r = *(s32*)ptr;
	ptr += 4;
	return r;
}

s32 readS32LE(void* &ptr) {
	s32 r = READ_LE_S32(ptr);
	ptr += 4;
	return r;
}

s32 readS32BE(void* &ptr) {
	s32 r = READ_BE_S32(ptr);
	ptr += 4;
	return r;
}

u32 readU32(void* &ptr) {
	u32 r = *(u32*)ptr;
	ptr += 4;
	return r;
}

u32 readU32LE(void* &ptr) {
	u32 r = READ_LE_U32(ptr);
	ptr += 4;
	return r;
}

u32 readU32BE(void* &ptr) {
	u32 r = READ_BE_U32(ptr);
	ptr += 4;
	return r;
} */
// #endregion void*
// #endregion Increment & Read
#endif