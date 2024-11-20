#include "hannibal.h"


////////////////////////////////////////////////// PIC BUILD
#ifdef PIC_BUILD


/**
 * Remember this is passing in the hannibal instance structure
 * from the main agent. The base address for this HBIN will be 
 * different.
 * 
 * The current buffer looks like this:
 *           8 Bytes             N Bytes
 * PTR To PTR > HBIN_IN struct | HBIN
 */

EXTERN_C SECTION_CODE VOID hbin_main(
    _In_ PVOID Param
) {

    PVOID base = StRipStart();
    PVOID in_struct = GetInStruct();

    HBIN_IN *in = *(HBIN_IN **)in_struct;
    LPVOID arg_buff = in->args;
    int arg_buff_size = in->arg_size;

    PINSTANCE hannibal_instance_ptr = in->hannibal_instance;


#else 

////////////////////////////////////////////////// DEBUG BUILD


/**
 * Not supported at the moment. You'll have to either load up a full instance or just remove
 * the hannibal_instance_ptr->Win32. part from function calls when testing.
 */

// INSTANCE hannibal_instance;
// PINSTANCE hannibal_instance_ptr = &hannibal_instance;

int main(
    _In_ PVOID Param
) {
    
#endif


    /**
     * The arg buffer is serialized in the order you passed them in via the Mythic Modal.
     * You will need to read them one at a time using the correct deserializer functions.
     * They must be read in the same order you passed them into Mythic.
     */

    LPVOID arg_buff_cursor = arg_buff;

    UINT32 test_arg_uint32 = ReadUint32(&arg_buff_cursor);
    CHAR *test_arg_string = ReadString(&arg_buff_cursor, arg_buff_size);
    WCHAR *test_arg_wstring = ReadStringW(&arg_buff_cursor, arg_buff_size);

    WCHAR uint32W[9] = {0};
    dword_to_wchar(test_arg_uint32, uint32W, 10);

    /**
     * For demonstration purposes, I'm ensuring that User32.dll is
     * loaded and these functions are resolved. If you know the agent
     * has already resolved these into the instance pointer then this is
     * not needed. If you call a function that has not been resolved you
     * will crash the agent. TODO: Hash strings.
     */

    hannibal_instance_ptr->Modules.User32 = hannibal_instance_ptr->Win32.LoadLibraryA("User32");
    hannibal_instance_ptr->Win32.MessageBoxA = get_func_ptr_from_module_eat(hannibal_instance_ptr->Modules.User32, HASH_STR("MessageBoxA"));
    hannibal_instance_ptr->Win32.MessageBoxW = get_func_ptr_from_module_eat(hannibal_instance_ptr->Modules.User32, HASH_STR("MessageBoxW"));

    hannibal_instance_ptr->Win32.MessageBoxW( NULL, (LPCWSTR)uint32W, L"ARG_1", MB_OK );
    hannibal_instance_ptr->Win32.MessageBoxA( NULL, (LPCSTR)test_arg_string, "ARG_2", MB_OK );
    hannibal_instance_ptr->Win32.MessageBoxW( NULL, (LPCWSTR)test_arg_wstring, L"ARG_3", MB_OK );

    
    
    /**
     * All contents of the response buffer must be encoded as wide characters.
     * Only a single wide null byte at the end. So effectively it is a single
     * WCHAR string. These examples show how to handle numbers.
     */
    
    WCHAR favabeans[] = L" - FROM INSIDE HBIN";

    UINT32 resp_size = sizeof(uint32W) + sizeof(favabeans);

    CHAR *res_buf = (CHAR *)hannibal_instance_ptr->Win32.VirtualAlloc(
        NULL, 
        resp_size, 
        MEM_COMMIT, 
        PAGE_READWRITE
    );

    
    pic_memcpy(res_buf, uint32W, sizeof(uint32W)); // There's some extra nulls in this so Mythic renders it a bit incorrect. TODO: Do this cleaner.
    pic_memcpy(res_buf + sizeof(uint32W), favabeans, sizeof(favabeans));

    /**
     * If you don't submit a task response
     * the task.uuid buffer won't get freed in post_tasking and will make a leak.
     * You have to alloc a new buffer, and copy some sort of WCHAR into it. Even just
     * L"OK" will do.
     */

    TASK response_task;
    response_task.task_uuid = in->controller_uuid;
    response_task.output = (LPCSTR)res_buf;
    response_task.output_size = resp_size;

    task_enqueue(hannibal_instance_ptr->tasks.tasks_response_queue, &response_task);


    /**
     * Remember the ReadString, ReadStringW functions alloc new memory.
     * Free them or you'll get memory leaks.
     */

    hannibal_instance_ptr->Win32.VirtualFree(test_arg_string, 0, MEM_RELEASE);
    hannibal_instance_ptr->Win32.VirtualFree(test_arg_wstring, 0, MEM_RELEASE);

}