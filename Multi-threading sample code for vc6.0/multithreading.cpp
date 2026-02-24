#include <windows.h>
#include <process.h>
#include <stdio.h>

CRITICAL_SECTION g_cs;   // synchronization object
int g_counter = 0;      // shared resource

// Thread function
unsigned __stdcall ThreadProc(void* pParam)
{
    int threadNum = *(int*)pParam;

    for (int i = 0; i < 5; i++)
    {
        EnterCriticalSection(&g_cs);

        g_counter++;
        printf("Thread %d -> counter = %d\n", threadNum, g_counter);

        LeaveCriticalSection(&g_cs);

        Sleep(500);
    }
    return 0;
}

int main()
{
    InitializeCriticalSection(&g_cs);

    const int THREAD_COUNT = 2;
    HANDLE hThreads[THREAD_COUNT];
    int threadIds[THREAD_COUNT];

    // Create threads
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threadIds[i] = i + 1;

        hThreads[i] = (HANDLE)_beginthreadex(
            NULL,           // security
            0,              // stack size
            ThreadProc,     // thread function
            &threadIds[i],  // parameter
            0,              // start immediately
            NULL            // thread ID
        );
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);

    // Cleanup
    for (int i = 0; i < THREAD_COUNT; i++)
        CloseHandle(hThreads[i]);

    DeleteCriticalSection(&g_cs);

    printf("All threads finished.\n");
    return 0;
}