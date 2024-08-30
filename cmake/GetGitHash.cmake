##############################################################################
## MADE BY NOTNHEAVY.                                                       ##
##############################################################################

# Load the Git package.
find_package(Git QUIET)
set(GETGITHASH_DIR ${CMAKE_CURRENT_LIST_DIR})

# Create a function for returning the Git hash at a specific directory.
# The parameter used following HASH_OUTPUT will be used as a custom working 
# directory. 
#
# This uses execute_process and therefore only works during the configuration
# stage if used standalone.
function(get_git_hash HASH_OUTPUT)
	# Check if Git is found on the system.
	if (NOT Git_FOUND)
		return()
	endif()

	# Resolve the working directory.
	set(CWD ${PROJECT_SOURCE_DIR})
	list(LENGTH ARGN ARGV_SURPLUS)
	if (${ARGV_SURPLUS} GREATER 0)
		list(GET ARGN 0 CWD)
	endif()

	# Execute Git to get the Git hash.
	execute_process(WORKING_DIRECTORY ${CWD}
		COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		OUTPUT_VARIABLE HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	set(${HASH_OUTPUT} ${HASH} PARENT_SCOPE)
endfunction()

# Create a new library which will transform an input file in order to write a
# Git hash during the build stage.
#s
# The name of the Git hash variable will be @${PROJECT_NAME}_GIT_HASH@
# The name of the library to link to will be ${PROJECT_NAME}_git_hash
function(git_transform_file PROJECT_NAME IN OUT)
	# Create a new target which will run this file in runtime mode.
	add_custom_target(${PROJECT_NAME}_runtime_get_git_hash COMMAND ${CMAKE_COMMAND} 
		-DRUNTIME_MODE=1
		-DIN=${IN}
		-DOUT=${OUT}
		-DPROJECT_NAME=${PROJECT_NAME}
		-P "${GETGITHASH_DIR}/GetGitHash.cmake"
	)

	# Create a new library for the source file containing the Git hash.
	add_library(${PROJECT_NAME}_git_hash ${OUT})
	add_dependencies(${PROJECT_NAME}_git_hash ${PROJECT_NAME}_runtime_get_git_hash)

	# Set the parameters given as parent variables and call get_git_hash_runtime()
	# to set up the final during the configuration stage.
	set(IN ${IN} PARENT_SCOPE)
	set(OUT ${OUT} PARENT_SCOPE)
	set(PROJECT_NAME ${PROJECT_NAME} PARENT_SCOPE)
	get_git_hash_runtime()
endfunction()

##############################################################################
## RUNTIME FUNCTIONS                                                        ##
##############################################################################

# Return the Git hash at a specific directory during the build stage.
function(get_git_hash_runtime)
	# Error if IN doesn't exist.
	if(NOT (EXISTS ${IN}))
		message(FATAL_ERROR "GetGitHash.cmake: input \"${IN}\" does not exist!")
	endif()

	# Resolve the directory of OUT.
	get_filename_component(DIR ${OUT} DIRECTORY)

	# Run get_git_hash() and transform the file.
	get_git_hash(${PROJECT_NAME}_GIT_HASH ${DIR})
	configure_file(${IN} ${OUT})
endfunction()

# Check if we are in runtime mode.
if(RUNTIME_MODE)
	get_git_hash_runtime()
endif()