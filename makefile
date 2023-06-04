################################################################################
########################## Make command instructions ###########################

##### Template #####
# command
#	description / defintiion

##### Terminal command list #####

### Defualt commands from template makefile ###
# make
#	Builds the first target it finds, which should be declared under the 'Main 
#	build recipe' heading. This command simply looks for the first command in
#	the file and starts trying to build it, further building any prerequisites
#	defined for the target. Only one recipe should exist under this heading.

# make tests
#	Builds all test files into executables and stores them in the build folder.
#	This function additionally calls 'build' first to make sure any detected
#	changes are built before building new tests, but nothing will happen if no
#	changes have been detected to save compilation time.

# make tests_and_runtests
#	Calls the 'tests' command defined above and then the 'run_tests' command
#	defined below.

# make clean
#	Removes all files from the 'build/tests' directory.

# make rebuild
#	Calls 'clean' and 'build' defined above.

# make rebuild_and_runtests
#	Calls 'rebuild_tests' defined above, and 'run_tests' defined below.

# make run_tests
#	Loops through all test executables in the 'build/tests' directory and runs
#	them, with an exit condition to exit the loop upon first failure.

# make make_directories
#	Create all required directories for this project. Typically only used to
#	create build directories but can be used for other purposes. Should be
#	defined as a prerequisite step for the first recipe called that builds any
#	type of file.

# make clear_log_files
#	Deletes all log files from within the various LogFiles directories.

# make announce_compiling_tests
#	Prints echo information to display when things are happening. Defined as a
#	prerequisite to the recipe that starts compiling test files.

################################################################################
########################## Basic variable definition ###########################

# Removal flags
RM := rm -rf

################################################################################
############################# Directory variables ##############################

# Install directory
at ?= /usr/local/include/dylanclibs

# Directories
#	Base directories
SRC_DIR ?= ./src
TEST_DIR ?= ./tests
BLD_DIR ?= ./build

#	Build directories
BLD_SRC_DIR ?= $(BLD_DIR)/src
BLD_TEST_DIR ?= $(BLD_DIR)/tests

################################################################################
##################### Retrieving and naming program files ######################

# Get the cpp files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Name the object files
OBJS := $(SRCS:$(SRC_DIR)/%=$(BLD_SRC_DIR)/obj/%.o)

# Get the test files
TEST_SRCS := $(shell find $(TEST_DIR) -name '*.cpp')

# Name test executables
TEST_EXECS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=%)

################################################################################
################################ Compiler flags ################################

################# C++ ##################

# Extra C++ Compiler Flags
#	-Wall = turn on all warnings
CXXFLAGS := -Wall

# Define the include directories for compiler
INC_DIRS := $(shell find $(SRC_DIR) -type d)
# Add the 'include' prefix to INC_DIRS
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

########################################

############### General ################

# C Preprocessor flags
# 	The -MMD and -MP flags together generate Makefiles for us!
# 	These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

########################################

################################################################################
############################ Other target recipe's #############################

target: tests

# Build test executables
$(TEST_EXECS): make_directories announce_compiling_tests
	@echo Building \"$@\" executable...
	@$(CXX) $(CXXFLAGS) -o $(BLD_TEST_DIR)/$@ $(TEST_DIR)/$@.cpp

################################################################################
################################ Phony targets #################################

# Define these custom commands
.PHONY: tests tests_and_runtests \
	clean \
	rebuild \
	rebuild_and_runtests \
	install \
	run_tests make_directories clear_log_files \
	announce_compiling_tests \
	single_test single_test_and_run

################################################################################
############################ Build command recipe's ############################

# CURRENT_TEST_TO_BUILD := string_test
# single_test:
# 	$(RM) $(BLD_TEST_DIR)/$(CURRENT_TEST_TO_BUILD)
# 	@echo Compiling $(CURRENT_TEST_TO_BUILD)
# 	@$(CXX) $(CXXFLAGS) -o $(BLD_TEST_DIR)/$(CURRENT_TEST_TO_BUILD) $(TEST_DIR)/$(CURRENT_TEST_TO_BUILD).cpp

# single_test_and_run: single_test
# 	$(BLD_TEST_DIR)/$(CURRENT_TEST_TO_BUILD) -s --reporter console || exit;

# Build test files
tests: $(TEST_EXECS)
	@echo Finished compiling tests, see output for details.
	@echo "####################################################################"

# Build tests and run them
tests_and_runtests: tests run_tests

################################################################################
############################ Clean command recipe's ############################

# Clean test files
clean:
	@echo "####################################################################"
	@echo Cleaning test files...
	@$(RM) $(BLD_TEST_DIR)/*
	@echo Test files cleaned.
	@echo "####################################################################"

################################################################################
########################### Rebuild command recipe's ###########################

# Rebuild test files - clean tests and build tests
rebuild: clean tests

# Rebuild test files and run them - rebuild tests and run tests
rebuild_and_runtests: rebuild run_tests

################################################################################
######################### Additional command recipe's ##########################

# Install the files into the includes directory
install:
	@echo Installing C++ Files to includes directory
	@mkdir -p $(at)
	@cp src/* $(at)

# Run test files
#	This assumes the test files have already been built by some other command.
#	-s --reporter console || exit	-	will exit upon error in tests
run_tests:
	@echo "####################################################################"
	@echo Running tests.....
	for exec in $(TEST_EXECS); do \
		$(BLD_TEST_DIR)/$$exec -s --reporter console || exit; \
	done
	@echo Tests finished running, see console output for details.
	@echo "####################################################################"

make_directories:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(BLD_DIR)/dbg
	@mkdir -p $(BLD_SRC_DIR)
	@mkdir -p $(BLD_SRC_DIR)/obj
	@mkdir -p $(BLD_TEST_DIR)

# Remove all the log files to clean the project folders.
clear_log_files:
	@echo "####################################################################"
	@echo Deleting log files...
	@$(RM) LogFiles/* $(BLD_SRC_DIR)/LogFiles/* $(BLD_TEST_DIR)/LogFiles/*
	@echo Log files deleted.
	@echo "####################################################################"

# Announce compiling tests has started.
announce_compiling_tests:
	@echo "####################################################################"
	@echo Compiling tests...

