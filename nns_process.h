//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

// TODO: linux/macos process code

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <stdbool.h>
#include "nns_macros.h"
#include "nns_utility.h"

//////////////////////////////////////////////////////////////////////////////
// GLOBALS                                                                  //
//////////////////////////////////////////////////////////////////////////////

// Default pipe buffer size.
#define PIPE_BUFFER_SIZE 256

// Process flags for StartProcess().
enum EProcessFlags
{
    PROCESS_NOCONSOLE = (1 << 0)
};

// Used for the StartProcess()::pipeout argument if you want to receive
// console output from the launched process. This will be called n times
// for each block of characters size PIPE_BUFFER_SIZE is found in the
// process' stdout at the end of its execution.
typedef void (*proc_pipeout)(const char* buffer, void* data);

// Use this for sharing data about pipeout.
typedef struct s_processdata
{
	proc_pipeout pipeout;
	void* pipeout_data;
} processdata;

//////////////////////////////////////////////////////////////////////////////
// PROCESS FUNCTIONS                                                        //
//////////////////////////////////////////////////////////////////////////////

// Basic process function. Only use it for basic process execution + stdout
// piping (if specified). Returns true on success, otherwise false.
static bool StartProcess(const char* path, const char* args, int flags, processdata* data)
{
	// Sort out command-line arguments.
	bool returnValue = true;
	char* exec = (char*)safe_malloc(strlen(path) + strlen(args) + 2);
    strcat(exec, path);
	strcat(exec, " ");
    strcat(exec, args);

	// Execute the process.
#if defined(_WIN32)
	// Set up process blocks.
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	// Create a pipe if pipeout isn't nullptr.
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	HANDLE read = INVALID_HANDLE_VALUE;
	HANDLE write = INVALID_HANDLE_VALUE;
	if (data)
	{
		RUNTIME_ERROR(CreatePipe(&read, &write, &sa, 0), "Could not create pipes: %s", returnValue = false; goto startprocess_error, strerror(errno));
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdOutput = write;
		si.hStdError = write;
	}

	// Launch the process and wait for it to finish.
	// CREATE_NO_WINDOW
	BOOL result = CreateProcessA(NULL, exec, NULL, NULL, data != NULL, ((flags & PROCESS_NOCONSOLE) ? CREATE_NO_WINDOW : 0), NULL, NULL, &si, &pi);
	RUNTIME_ERROR(result, "Failed to launch process \"%s\": %s\n", returnValue = false; goto startprocess_error, path, strerror(errno));
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Read from the stdout pip if pipeout isn't nullptr.
	if (data->pipeout)
	{
		CloseHandle(write);
		for (;;)
		{
			// Read into a buffer.
			char buffer[PIPE_BUFFER_SIZE + 1] = { 0 };
			DWORD readWord;
			if (ReadFile(read, buffer, sizeof(buffer) - 1, &readWord, NULL))
			{
				buffer[readWord] = '\0';
                data->pipeout(buffer, data->pipeout_data);
			}
			else
				break;
		}
	}

	// Close all handles.
startprocess_error:
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (data)
		CloseHandle(read);
#elif defined(__linux__)

#else
	// TODO: SUPPORT MACOS!
	assert(0);
#endif
	return returnValue;
}

#ifdef __cplusplus
}
#endif