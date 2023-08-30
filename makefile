################################################################################
########################## Make command instructions ###########################

# make
#	The default target recipe for this project is uniquely just for test files,
#	as there is no main executable that can be built in this project. 'make' and
#	'make tests' have the same effect. See further make recipes below.

# make tests
#	As mentioned above.

# make tests_and_runtests
#	Calls the 'make tests' command defined above and then the 'make run_tests'
#	command defined below.

# make clean
#	Removes all files from the 'build/tests' directory.

# make rebuild
#	Calls 'make clean' and 'make tests' defined above.

# make rebuild_and_runtests
#	Calls 'make rebuild_tests' defined above, and 'make run_tests' defined
#	below.

# make install
#	This command needs to be called with 'sudo' as it will try to make
#	directories in restricted locations and copy files to the direcotries it
#	creates. Follow 'sudo make install' with a space delimited list to further
#	define the 'src_at', 'bin_at' and 'lint_config_at' variables to copy source
#	files to the 'src_at' direcotry, binary files to the 'bin_at' directory, and
#	any cppnamelint config files to the 'lint_config_at' directory.

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

# make validate_cpp_lint (EXPERIMENTAL)
#	Validates the code in all source files using the defined .toml file in the
#	recipe command.

################################################################################
########################## Basic variable definition ###########################

# Make directory with -p flag
MKDIR_P := mkdir -p

# Removal flags
RM := rm -rf

# Define a single test file to build, defined without the '.cpp' extension so it
#	can build the correctly named binary
file ?= 

################################################################################
############################# Directory variables ##############################

# Install directory
src_at ?= /usr/local/include/dylanclibs
bin_at ?= /usr/bin/
lint_config_at ?= /usr/local/bin/lint_config

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
############################# Main Target Recipe ###############################

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
	rebuild rebuild_and_runtests \
	install \
	run_tests make_directories clear_log_files \
	announce_compiling_tests \
	validate_cpp_lint


################################################################################
############################ Build command recipe's ############################

# Build test files
tests: make_directories announce_compiling_tests
	@if [$(file) == ""]; then \
		for exec in $(TEST_EXECS); do \
			echo Building \"$$exec\" executable...; \
			$(CXX) $(CXXFLAGS) -o $(BLD_TEST_DIR)/$$exec $(TEST_DIR)/$$exec.cpp; \
		done \
	else \
		$(CXX) $(CXXFLAGS) -o $(BLD_TEST_DIR)/$(file) $(TEST_DIR)/$(file).cpp; \
	fi
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
	@echo Installing C++ Files to \"$(src_at)\"
	@$(MKDIR_P) $(src_at)
	@cp src/*.h* $(src_at)
	@echo Installing third party libraries to local include directory
	@cp src/RapidXML/* /usr/local/include
	@cp -r src/nlohmann /usr/local/include
	@cp -r src/catch2 /usr/local/include
	@cp -r src/HTTPRequest /usr/local/include
	@echo Installing binaries to \"$(bin_at)\"
	@$(MKDIR_P) $(bin_at)
	@cp bin/* $(bin_at)
	@echo Moving lint config files to \"$(lint_config_at)\"
	@$(MKDIR_P) $(lint_config_at)
	@cp lint_config/* $(lint_config_at)

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
	@$(MKDIR_P) $(BLD_DIR)
	@$(MKDIR_P) $(BLD_DIR)/dbg
	@$(MKDIR_P) $(BLD_SRC_DIR)
	@$(MKDIR_P) $(BLD_SRC_DIR)/obj
	@$(MKDIR_P) $(BLD_TEST_DIR)

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

# Validate the source code against the .toml config files.
validate_cpp_lint:
	@echo "####################################################################"
	@echo Validating C++ code via \"cppnamelint\"...
	for cppfile in $(SRCS); do \
		cppnamelint check $$cppfile -config /usr/local/bin/lint_config/dylan_convention.toml; \
	done
	@echo "####################################################################"

