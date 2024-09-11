//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

// TODO: linux/macos process code

#pragma once

#include "nns_memory.h"
#include "nns_os.h"

//////////////////////////////////////////////////////////////////////////////
// GLOBALS                                                                  //
//////////////////////////////////////////////////////////////////////////////

// Default pipe buffer size.
#define NNS_PIPE_BUFFER_SIZE	256

// Unix-only.
#define DESCRIPTORS_WRITE		1
#define DESCRIPTORS_READ		0

// Used for the StartProcess()::data argument if you want to receive console 
// output from the launched process. This will be called n times for each block 
// of characters size PIPE_BUFFER_SIZE is found in the process' stdout at the 
// end of its execution.
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
// piping (if specified). `data` can be NULL. `flags` is Windows-only (for
// CreateProcess). Returns true on success, otherwise false.
STOCK static bool NNS_StartProcess(char* argv[], STOCK int flags, processdata* data)
{
	// Init.
	bool returnValue = false;

	// Execute the process.
#if defined(_WIN32)
	// Sort out command-line arguments.
	size_t size = 0;
	for (char** ptr = argv; *ptr != NULL; ++ptr)
		size += strlen(*ptr) + 1;
	char* exec = (char*)NNS_malloc(size);
	for (char** ptr = argv; *ptr != NULL; ++ptr)
	{
		strcat(exec, *ptr);
		strcat(exec, " ");
	}
	exec[size - 1] = '\0';

	// Set up process blocks.
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	// Create read/write pipes if data isn't NULL.
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	HANDLE readpipe = INVALID_HANDLE_VALUE;
	HANDLE writepipe = INVALID_HANDLE_VALUE;
	if (data)
	{
		RUNTIME_ERROR(CreatePipe(&readpipe, &writepipe, &sa, 0), "Could not create pipes: %s", goto startprocess_error, strerror(errno));
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdOutput = writepipe;
		si.hStdError = writepipe;
	}

	// Launch the process and wait for it to finish.
	BOOL result = CreateProcessA(NULL, exec, NULL, NULL, data != NULL, flags, NULL, NULL, &si, &pi);
	RUNTIME_ERROR(result, "Failed to launch process \"%s\": %s\n", goto startprocess_error, argv[0], strerror(errno));
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Read from the stdout pipe if data->pipeout isn't NULL.
	if (data && data->pipeout)
	{
		CloseHandle(writepipe);
		writepipe = INVALID_HANDLE_VALUE;
		for (;;)
		{
			// Read into a buffer.
			char buffer[NNS_PIPE_BUFFER_SIZE + 1] = { 0 };
			DWORD readWord;
			if (ReadFile(readpipe, buffer, sizeof(buffer) - 1, &readWord, NULL))
			{
				buffer[readWord] = '\0';
                data->pipeout(buffer, data->pipeout_data);
			}
			else
				break;
		}
	}
	returnValue = true;

	// Close all handles.
startprocess_error:
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (readpipe != INVALID_HANDLE_VALUE)
		CloseHandle(readpipe);
	if (writepipe != INVALID_HANDLE_VALUE)
		CloseHandle(writepipe);
#elif defined(NNS_ISUNIX)
	// Create read/write pipes if pipeout isn't NULL.
	int descriptors[2];
	if (data)
		RUNTIME_ERROR(pipe(descriptors) == 0, "Could not create pipes: %s\n", return false, strerror(errno));

	// Configure the process.
	posix_spawn_file_actions_t actions;
	posix_spawn_file_actions_init(&actions);
	if (data)
	{
		posix_spawn_file_actions_adddup2(&actions, descriptors[DESCRIPTORS_WRITE], STDOUT_FILENO);
		posix_spawn_file_actions_addclose(&actions, descriptors[DESCRIPTORS_WRITE]);
		posix_spawn_file_actions_addclose(&actions, descriptors[DESCRIPTORS_READ]);
	}

	// Launch the process and wait for it to finish.
	pid_t pid;
	int status = posix_spawnp(&pid, argv[0], &actions, NULL, argv, __environ);
	RUNTIME_ERROR(waitpid(pid, &status, 0) != -1, "Failed to launch process \"%s\": %s\n", goto spawn_error, argv[0], strerror(errno));
	fflush(NULL);

	// Read from the stdout pipe if data->pipeout isn't NULL.
	close(descriptors[DESCRIPTORS_WRITE]);
	if (data && data->pipeout)
	{
		for (;;)
		{
			// Read into a buffer.
			char buffer[NNS_PIPE_BUFFER_SIZE + 1] = { 0 };
			ssize_t readnum = read(descriptors[DESCRIPTORS_READ], buffer, sizeof(buffer) - 1);
			RUNTIME_ERROR(readnum != -1, "read() failed: %s", goto spawn_error, strerror(errno));
			if (readnum > 0)
			{
				buffer[readnum] = '\0';
                data->pipeout(buffer, data->pipeout_data);
			}
			else
				break;
			break;
		}
	}
	returnValue = true;

spawn_error:
	posix_spawn_file_actions_destroy(&actions);
#endif

	// Return.
	return returnValue;
}