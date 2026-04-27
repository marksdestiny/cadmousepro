set(EXECUTABLE_FILE "cadmousepro")

# Qt libraries:
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files
find_package(Qt5 CONFIG REQUIRED Core DBus)

# USB libraries:
INCLUDE(FindPkgConfig)
pkg_check_modules (USB_LIBS REQUIRED hidapi-hidraw)
include_directories(${USB_LIBS_INCLUDE_DIRS})
link_directories(${USB_LIBS_LIBRARY_DIRS})

# Add ASan AddressSanitizer
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

# Generate C++ interface classes from D-Bus XML:
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/)
set(DBUS_XML ${CMAKE_CURRENT_SOURCE_DIR}/../../d-bus/info.globalcode.mouse.cadMousePro.xml)
set(GENERATED_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/)
add_custom_command(
	DEPENDS ${DBUS_XML}
	OUTPUT ${GENERATED_SOURCES}/DBusAdaptor.h
	OUTPUT ${GENERATED_SOURCES}/DBusAdaptor.cpp
	COMMAND qdbusxml2cpp -a DBusAdaptor.h:DBusAdaptor.cpp -c DBusAdaptor -l Daemon -i Daemon.h ${DBUS_XML}
	WORKING_DIRECTORY ${GENERATED_SOURCES}
)
cmake_policy(SET CMP0071 NEW) # Let AUTOMOC and AUTOUIC process GENERATED files.

# Executable output:
add_executable(
	${EXECUTABLE_FILE}
	Daemon.h
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/DBusAdaptor.h
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/DBusAdaptor.cpp
	Daemon.cpp
	cadMousePro.cpp
)

# Link libraries:
target_link_libraries(${EXECUTABLE_FILE} ${USB_LIBS_LIBRARIES} Qt5::DBus)
set_property(TARGET ${EXECUTABLE_FILE} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
