#include "utility_serialization.h"


SECTION_CODE void WriteUint8(UINT8 **buffer, UINT8 value) 
{
    **buffer = value;
    (*buffer)++;
}

SECTION_CODE void WriteUint32(UINT8 **buffer, UINT32 value) 
{ 
    // Ensure little-endian encoding
    for (int i = 0; i < 4; i++) {
        **buffer = (UINT8)(value & 0xFF); // Write the least significant byte first
        (*buffer)++;
        value >>= 8; // Shift right to get the next byte
    }
}

SECTION_CODE void WriteUint64(UINT8 **buffer, UINT64 value) 
{
    for (size_t i = 0; i < sizeof(UINT64); i++) {
        **buffer = (UINT8)(value & 0xFF); 
        (*buffer)++;
        value >>= 8; 
    }
}

SECTION_CODE void WriteString(UINT8 **buffer, const char *str, BOOL include_null)
{
    while (*str) {
        **buffer = *str;
        (*buffer)++;
        str++;
    }
    if (include_null){
        **buffer = 0;
        (*buffer)++;
    }
}

SECTION_CODE void WriteStringW(UINT8 **buffer, const wchar_t *str, BOOL include_null)
{
    while (*str) {
        // Write each wide character (2 bytes)
        **buffer = (UINT8)(*str & 0xFF);         // Lower byte
        (*buffer)++;
        **buffer = (UINT8)((*str >> 8) & 0xFF); // Upper byte
        (*buffer)++;
        str++;
    }
    if (include_null) {
        // Write the null terminator (0x0000) as 2 bytes
        **buffer = 0x00;
        (*buffer)++;
        **buffer = 0x00;
        (*buffer)++;
    }
}

SECTION_CODE void WriteBytes(UINT8 **buffer, const char *str, int size)
{
    for (int i = 0; i < size; i++){
        **buffer = *str;
        (*buffer)++;
        str++;
    }
}

SECTION_CODE UINT8 ReadUint8(UINT8 **buffer) 
{
    UINT8 value = **buffer;
    (*buffer)++;
    return value;
}

/**
 * TODO: Ensure we don't read past end of arg buffer
 */
SECTION_CODE UINT32 ReadUint32(UINT8 **buffer) 
{
    UINT32 value = 0;
    for (int i = 0; i < 4; i++) {
        value |= ((UINT32)**buffer) << (8 * i);
        (*buffer)++;
    }
    return value;
}

SECTION_CODE UINT8* ReadBytes(UINT8 **buffer, UINT32 length) 
{
    HANNIBAL_INSTANCE_PTR

    UINT8* bytes = (UINT8*)hannibal_instance_ptr->Win32.VirtualAlloc(NULL, length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!bytes) return NULL;

    pic_memcpy(bytes, *buffer, length);

    *buffer += length;

    return bytes;
}

SECTION_CODE char* ReadString(UINT8 **buffer, UINT32 length) 
{
    HANNIBAL_INSTANCE_PTR

    UINT32 bytesRead = 0;

    while (bytesRead < length && (*buffer)[bytesRead] != 0x00) {
        bytesRead++;
    }

    if (bytesRead == length) {
        return NULL;  
    }

    char* str = (char*)hannibal_instance_ptr->Win32.VirtualAlloc(NULL, bytesRead + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!str) return NULL;

    pic_memcpy(str, *buffer, bytesRead + 1);  // +1 to include the null byte

    *buffer += bytesRead + 1;

    return str;
}


SECTION_CODE wchar_t* ReadStringW(UINT8 **buffer, UINT32 length) 
{
    HANNIBAL_INSTANCE_PTR

    UINT32 wcharCount = 0;

    // Loop to find the null wide character (L'\0' which is 0x00 0x00 in UTF-16)
    while (wcharCount < length / 2) {  // length is in bytes, so divide by 2 for wchar_t units
        if (((*buffer)[wcharCount * 2] == 0x00) && ((*buffer)[wcharCount * 2 + 1] == 0x00)) {
            break;
        }
        wcharCount++;
    }

    // If no null wide character found, return NULL (or handle error)
    if (wcharCount == length / 2) {
        return NULL;
    }

    wchar_t* str = (wchar_t*)hannibal_instance_ptr->Win32.VirtualAlloc(NULL, (wcharCount + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!str) return NULL;

    pic_memcpy(str, *buffer, (wcharCount + 1) * sizeof(wchar_t));  // +1 to include the null wchar_t

    *buffer += (wcharCount + 1) * sizeof(wchar_t);

    return str;
}