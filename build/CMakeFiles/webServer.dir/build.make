# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/githubPorject/WebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/githubPorject/WebServer/build

# Include any dependencies generated for this target.
include CMakeFiles/webServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/webServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/webServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webServer.dir/flags.make

CMakeFiles/webServer.dir/src/buff.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/buff.cpp.o: /root/githubPorject/WebServer/src/buff.cpp
CMakeFiles/webServer.dir/src/buff.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webServer.dir/src/buff.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/buff.cpp.o -MF CMakeFiles/webServer.dir/src/buff.cpp.o.d -o CMakeFiles/webServer.dir/src/buff.cpp.o -c /root/githubPorject/WebServer/src/buff.cpp

CMakeFiles/webServer.dir/src/buff.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/buff.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/buff.cpp > CMakeFiles/webServer.dir/src/buff.cpp.i

CMakeFiles/webServer.dir/src/buff.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/buff.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/buff.cpp -o CMakeFiles/webServer.dir/src/buff.cpp.s

CMakeFiles/webServer.dir/src/epoller.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/epoller.cpp.o: /root/githubPorject/WebServer/src/epoller.cpp
CMakeFiles/webServer.dir/src/epoller.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webServer.dir/src/epoller.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/epoller.cpp.o -MF CMakeFiles/webServer.dir/src/epoller.cpp.o.d -o CMakeFiles/webServer.dir/src/epoller.cpp.o -c /root/githubPorject/WebServer/src/epoller.cpp

CMakeFiles/webServer.dir/src/epoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/epoller.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/epoller.cpp > CMakeFiles/webServer.dir/src/epoller.cpp.i

CMakeFiles/webServer.dir/src/epoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/epoller.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/epoller.cpp -o CMakeFiles/webServer.dir/src/epoller.cpp.s

CMakeFiles/webServer.dir/src/heaptimer.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/heaptimer.cpp.o: /root/githubPorject/WebServer/src/heaptimer.cpp
CMakeFiles/webServer.dir/src/heaptimer.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webServer.dir/src/heaptimer.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/heaptimer.cpp.o -MF CMakeFiles/webServer.dir/src/heaptimer.cpp.o.d -o CMakeFiles/webServer.dir/src/heaptimer.cpp.o -c /root/githubPorject/WebServer/src/heaptimer.cpp

CMakeFiles/webServer.dir/src/heaptimer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/heaptimer.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/heaptimer.cpp > CMakeFiles/webServer.dir/src/heaptimer.cpp.i

CMakeFiles/webServer.dir/src/heaptimer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/heaptimer.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/heaptimer.cpp -o CMakeFiles/webServer.dir/src/heaptimer.cpp.s

CMakeFiles/webServer.dir/src/httpconnection.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/httpconnection.cpp.o: /root/githubPorject/WebServer/src/httpconnection.cpp
CMakeFiles/webServer.dir/src/httpconnection.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webServer.dir/src/httpconnection.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/httpconnection.cpp.o -MF CMakeFiles/webServer.dir/src/httpconnection.cpp.o.d -o CMakeFiles/webServer.dir/src/httpconnection.cpp.o -c /root/githubPorject/WebServer/src/httpconnection.cpp

CMakeFiles/webServer.dir/src/httpconnection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/httpconnection.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/httpconnection.cpp > CMakeFiles/webServer.dir/src/httpconnection.cpp.i

CMakeFiles/webServer.dir/src/httpconnection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/httpconnection.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/httpconnection.cpp -o CMakeFiles/webServer.dir/src/httpconnection.cpp.s

CMakeFiles/webServer.dir/src/httprequest.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/httprequest.cpp.o: /root/githubPorject/WebServer/src/httprequest.cpp
CMakeFiles/webServer.dir/src/httprequest.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webServer.dir/src/httprequest.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/httprequest.cpp.o -MF CMakeFiles/webServer.dir/src/httprequest.cpp.o.d -o CMakeFiles/webServer.dir/src/httprequest.cpp.o -c /root/githubPorject/WebServer/src/httprequest.cpp

CMakeFiles/webServer.dir/src/httprequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/httprequest.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/httprequest.cpp > CMakeFiles/webServer.dir/src/httprequest.cpp.i

CMakeFiles/webServer.dir/src/httprequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/httprequest.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/httprequest.cpp -o CMakeFiles/webServer.dir/src/httprequest.cpp.s

CMakeFiles/webServer.dir/src/httpresponse.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/httpresponse.cpp.o: /root/githubPorject/WebServer/src/httpresponse.cpp
CMakeFiles/webServer.dir/src/httpresponse.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webServer.dir/src/httpresponse.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/httpresponse.cpp.o -MF CMakeFiles/webServer.dir/src/httpresponse.cpp.o.d -o CMakeFiles/webServer.dir/src/httpresponse.cpp.o -c /root/githubPorject/WebServer/src/httpresponse.cpp

CMakeFiles/webServer.dir/src/httpresponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/httpresponse.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/httpresponse.cpp > CMakeFiles/webServer.dir/src/httpresponse.cpp.i

CMakeFiles/webServer.dir/src/httpresponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/httpresponse.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/httpresponse.cpp -o CMakeFiles/webServer.dir/src/httpresponse.cpp.s

CMakeFiles/webServer.dir/src/log.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/log.cpp.o: /root/githubPorject/WebServer/src/log.cpp
CMakeFiles/webServer.dir/src/log.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webServer.dir/src/log.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/log.cpp.o -MF CMakeFiles/webServer.dir/src/log.cpp.o.d -o CMakeFiles/webServer.dir/src/log.cpp.o -c /root/githubPorject/WebServer/src/log.cpp

CMakeFiles/webServer.dir/src/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/log.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/log.cpp > CMakeFiles/webServer.dir/src/log.cpp.i

CMakeFiles/webServer.dir/src/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/log.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/log.cpp -o CMakeFiles/webServer.dir/src/log.cpp.s

CMakeFiles/webServer.dir/src/main.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/main.cpp.o: /root/githubPorject/WebServer/src/main.cpp
CMakeFiles/webServer.dir/src/main.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/webServer.dir/src/main.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/main.cpp.o -MF CMakeFiles/webServer.dir/src/main.cpp.o.d -o CMakeFiles/webServer.dir/src/main.cpp.o -c /root/githubPorject/WebServer/src/main.cpp

CMakeFiles/webServer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/main.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/main.cpp > CMakeFiles/webServer.dir/src/main.cpp.i

CMakeFiles/webServer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/main.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/main.cpp -o CMakeFiles/webServer.dir/src/main.cpp.s

CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o: /root/githubPorject/WebServer/src/sqlconnectionpool.cpp
CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o -MF CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o.d -o CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o -c /root/githubPorject/WebServer/src/sqlconnectionpool.cpp

CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/sqlconnectionpool.cpp > CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.i

CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/sqlconnectionpool.cpp -o CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.s

CMakeFiles/webServer.dir/src/webserver.cpp.o: CMakeFiles/webServer.dir/flags.make
CMakeFiles/webServer.dir/src/webserver.cpp.o: /root/githubPorject/WebServer/src/webserver.cpp
CMakeFiles/webServer.dir/src/webserver.cpp.o: CMakeFiles/webServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/webServer.dir/src/webserver.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/webServer.dir/src/webserver.cpp.o -MF CMakeFiles/webServer.dir/src/webserver.cpp.o.d -o CMakeFiles/webServer.dir/src/webserver.cpp.o -c /root/githubPorject/WebServer/src/webserver.cpp

CMakeFiles/webServer.dir/src/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webServer.dir/src/webserver.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/githubPorject/WebServer/src/webserver.cpp > CMakeFiles/webServer.dir/src/webserver.cpp.i

CMakeFiles/webServer.dir/src/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webServer.dir/src/webserver.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/githubPorject/WebServer/src/webserver.cpp -o CMakeFiles/webServer.dir/src/webserver.cpp.s

# Object files for target webServer
webServer_OBJECTS = \
"CMakeFiles/webServer.dir/src/buff.cpp.o" \
"CMakeFiles/webServer.dir/src/epoller.cpp.o" \
"CMakeFiles/webServer.dir/src/heaptimer.cpp.o" \
"CMakeFiles/webServer.dir/src/httpconnection.cpp.o" \
"CMakeFiles/webServer.dir/src/httprequest.cpp.o" \
"CMakeFiles/webServer.dir/src/httpresponse.cpp.o" \
"CMakeFiles/webServer.dir/src/log.cpp.o" \
"CMakeFiles/webServer.dir/src/main.cpp.o" \
"CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o" \
"CMakeFiles/webServer.dir/src/webserver.cpp.o"

# External object files for target webServer
webServer_EXTERNAL_OBJECTS =

webServer: CMakeFiles/webServer.dir/src/buff.cpp.o
webServer: CMakeFiles/webServer.dir/src/epoller.cpp.o
webServer: CMakeFiles/webServer.dir/src/heaptimer.cpp.o
webServer: CMakeFiles/webServer.dir/src/httpconnection.cpp.o
webServer: CMakeFiles/webServer.dir/src/httprequest.cpp.o
webServer: CMakeFiles/webServer.dir/src/httpresponse.cpp.o
webServer: CMakeFiles/webServer.dir/src/log.cpp.o
webServer: CMakeFiles/webServer.dir/src/main.cpp.o
webServer: CMakeFiles/webServer.dir/src/sqlconnectionpool.cpp.o
webServer: CMakeFiles/webServer.dir/src/webserver.cpp.o
webServer: CMakeFiles/webServer.dir/build.make
webServer: /usr/lib64/mysql/libmysqlclient.a
webServer: CMakeFiles/webServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/githubPorject/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable webServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webServer.dir/build: webServer
.PHONY : CMakeFiles/webServer.dir/build

CMakeFiles/webServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webServer.dir/clean

CMakeFiles/webServer.dir/depend:
	cd /root/githubPorject/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/githubPorject/WebServer /root/githubPorject/WebServer /root/githubPorject/WebServer/build /root/githubPorject/WebServer/build /root/githubPorject/WebServer/build/CMakeFiles/webServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webServer.dir/depend
