#include "common.h"

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
AutoAdvancePtr::AutoAdvancePtr(u8* ptr) { this->ptr = ptr; }
void AutoAdvancePtr::initialize(u8* ptr) { this->ptr = ptr; }
s8 AutoAdvancePtr::readS8() { return (s8)(*(ptr++)); }
s8 AutoAdvancePtr::readS8LE() { return (s8)(*(ptr++)); }
s8 AutoAdvancePtr::readS8BE() { return (s8)(*(ptr++)); }
u8 AutoAdvancePtr::readU8() { return *(ptr++); }
u8 AutoAdvancePtr::readU8LE() { return *(ptr++); }
u8 AutoAdvancePtr::readU8BE() { return *(ptr++); }
s16 AutoAdvancePtr::readS16()
{
	s16 r = *(s16*)ptr;
	ptr += 2;
	return r;
}
s16 AutoAdvancePtr::readS16LE()
{
	s16 r = READ_LE_S16(ptr);
	ptr += 2;
	return r;
}
s16 AutoAdvancePtr::readS16BE()
{
	s16 r = READ_BE_S16(ptr);
	ptr += 2;
	return r;
}
u16 AutoAdvancePtr::readU16()
{
	u16 r = *(u16*)ptr;
	ptr += 2;
	return r;
}
u16 AutoAdvancePtr::readU16LE()
{
	u16 r = READ_LE_U16(ptr);
	ptr += 2;
	return r;
}
u16 AutoAdvancePtr::readU16BE()
{
	u16 r = READ_BE_U16(ptr);
	ptr += 2;
	return r;
}
s32 AutoAdvancePtr::readS32()
{
	s32 r = *(s32*)ptr;
	ptr += 4;
	return r;
}
s32 AutoAdvancePtr::readS32LE()
{
	s32 r = READ_LE_S32(ptr);
	ptr += 4;
	return r;
}
s32 AutoAdvancePtr::readS32BE()
{
	s32 r = READ_BE_S32(ptr);
	ptr += 4;
	return r;
}
u32 AutoAdvancePtr::readU32()
{
	u32 r = *(u32*)ptr;
	ptr += 4;
	return r;
}
u32 AutoAdvancePtr::readU32LE()
{
	u32 r = READ_LE_U32(ptr);
	ptr += 4;
	return r;
}
u32 AutoAdvancePtr::readU32BE()
{
	u32 r = READ_BE_U32(ptr);
	ptr += 4;
	return r;
}
s8 AutoAdvancePtr::readOnlyS8() { return (s8)(*ptr); }
s8 AutoAdvancePtr::readOnlyS8LE() { return (s8)(*ptr); }
s8 AutoAdvancePtr::readOnlyS8BE() { return (s8)(*ptr); }
u8 AutoAdvancePtr::readOnlyU8() { return *ptr; }
u8 AutoAdvancePtr::readOnlyU8LE() { return *ptr; }
u8 AutoAdvancePtr::readOnlyU8BE() { return *ptr; }
s16 AutoAdvancePtr::readOnlyS16() { return *(s16*)ptr; }
s16 AutoAdvancePtr::readOnlyS16LE() { return READ_LE_S16(ptr); }
s16 AutoAdvancePtr::readOnlyS16BE() { return READ_BE_S16(ptr); }
u16 AutoAdvancePtr::readOnlyU16() { return *(u16*)ptr; }
u16 AutoAdvancePtr::readOnlyU16LE() { return READ_LE_U16(ptr); }
u16 AutoAdvancePtr::readOnlyU16BE() { return READ_BE_U16(ptr); }
s32 AutoAdvancePtr::readOnlyS32() { return *(s32*)ptr; }
s32 AutoAdvancePtr::readOnlyS32LE() { return READ_LE_S32(ptr); }
s32 AutoAdvancePtr::readOnlyS32BE() { return READ_BE_S32(ptr); }
u32 AutoAdvancePtr::readOnlyU32() { return *(u32*)ptr; }
u32 AutoAdvancePtr::readOnlyU32LE() { return READ_LE_U32(ptr); }
u32 AutoAdvancePtr::readOnlyU32BE() { return READ_BE_U32(ptr); }
// }; typedef class AutoAdvancePtr AutoAdvancePtr;
s8 readS8(u8*& ptr) { return ((s8)(*(ptr++))); }
s8 readS8(s8*& ptr) { return ((s8)(*(ptr++))); }
s8 readS8LE(u8*& ptr) { return ((s8)(*(ptr++))); }
s8 readS8LE(s8*& ptr) { return ((s8)(*(ptr++))); }
s8 readS8BE(u8*& ptr) { return ((s8)(*(ptr++))); }
s8 readS8BE(s8*& ptr) { return ((s8)(*(ptr++))); }
u8 readU8(u8*& ptr) { return ((u8)(*(ptr++))); }
u8 readU8(s8*& ptr) { return ((u8)(*(ptr++))); }
u8 readU8LE(u8*& ptr) { return ((u8)(*(ptr++))); }
u8 readU8LE(s8*& ptr) { return ((u8)(*(ptr++))); }
u8 readU8BE(u8*& ptr) { return ((u8)(*(ptr++))); }
u8 readU8BE(s8*& ptr) { return ((u8)(*(ptr++))); }
s16 readS16(u8*& ptr)
{
	s16 r = *(s16*)ptr;
	ptr = &ptr[2];
	return r;
}
s16 readS16(s8*& ptr)
{
	s16 r = *(s16*)ptr;
	ptr = &ptr[2];
	return r;
}
s16 readS16LE(u8*& ptr)
{
	s16 r = READ_LE_S16(ptr);
	ptr = &ptr[2];
	return r;
}
s16 readS16LE(s8*& ptr)
{
	s16 r = READ_LE_S16(ptr);
	ptr = &ptr[2];
	return r;
}
s16 readS16BE(u8*& ptr)
{
	s16 r = READ_BE_S16(ptr);
	ptr = &ptr[2];
	return r;
}
s16 readS16BE(s8*& ptr)
{
	s16 r = READ_BE_S16(ptr);
	ptr = &ptr[2];
	return r;
}
u16 readU16(u8*& ptr)
{
	u16 r = *(u16*)ptr;
	ptr = &ptr[2];
	return r;
}
u16 readU16(s8*& ptr)
{
	u16 r = *(u16*)ptr;
	ptr = &ptr[2];
	return r;
}
u16 readU16LE(u8*& ptr)
{
	u16 r = READ_LE_U16(ptr);
	ptr = &ptr[2];
	return r;
}
u16 readU16LE(s8*& ptr)
{
	u16 r = READ_LE_U16(ptr);
	ptr = &ptr[2];
	return r;
}
u16 readU16BE(u8*& ptr)
{
	u16 r = READ_BE_U16(ptr);
	ptr = &ptr[2];
	return r;
}
u16 readU16BE(s8*& ptr)
{
	u16 r = READ_BE_U16(ptr);
	ptr = &ptr[2];
	return r;
}
s32 readS32(u8*& ptr)
{
	s32 r = *(s32*)ptr;
	ptr += 4;
	return r;
}
s32 readS32(s8*& ptr)
{
	s32 r = *(s32*)ptr;
	ptr += 4;
	return r;
}
s32 readS32LE(u8*& ptr)
{
	s32 r = READ_LE_S32(ptr);
	ptr += 4;
	return r;
}
s32 readS32LE(s8*& ptr)
{
	s32 r = READ_LE_S32(ptr);
	ptr += 4;
	return r;
}
s32 readS32BE(u8*& ptr)
{
	s32 r = READ_BE_S32(ptr);
	ptr += 4;
	return r;
}
s32 readS32BE(s8*& ptr)
{
	s32 r = READ_BE_S32(ptr);
	ptr += 4;
	return r;
}
u32 readU32(u8*& ptr)
{
	u32 r = *(u32*)ptr;
	ptr += 4;
	return r;
}
u32 readU32(s8*& ptr)
{
	u32 r = *(u32*)ptr;
	ptr += 4;
	return r;
}
u32 readU32LE(u8*& ptr)
{
	u32 r = READ_LE_U32(ptr);
	ptr += 4;
	return r;
}
u32 readU32LE(s8*& ptr)
{
	u32 r = READ_LE_U32(ptr);
	ptr += 4;
	return r;
}
u32 readU32BE(u8*& ptr)
{
	u32 r = READ_BE_U32(ptr);
	ptr += 4;
	return r;
}
u32 readU32BE(s8*& ptr)
{
	u32 r = READ_BE_U32(ptr);
	ptr += 4;
	return r;
}
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
