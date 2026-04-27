set(EXECUTABLE_FILE "cadMousePro-gui")

# Qt libraries:
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(Qt5Widgets CONFIG REQUIRED)

# Add ASan AddressSanitizer
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

# Generate C++ interface classes from D-Bus XML:
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/)
set(GENERATED_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/generated-sources/)

set(DBUS_XML ${CMAKE_CURRENT_SOURCE_DIR}/../../d-bus/info.globalcode.mouse.cadMousePro.xml)
add_custom_command(
	DEPENDS ${DBUS_XML}
	OUTPUT ${GENERATED_SOURCES}/Proxy.h
	OUTPUT ${GENERATED_SOURCES}/Proxy.cpp
	COMMAND qdbusxml2cpp -p Proxy.h:Proxy.cpp ${DBUS_XML}
	WORKING_DIRECTORY ${GENERATED_SOURCES}
)

set(DBUS_XML ${CMAKE_CURRENT_SOURCE_DIR}/../../d-bus/org.freedesktop.UPower.xml)
add_custom_command(
	DEPENDS ${DBUS_XML}
	OUTPUT ${GENERATED_SOURCES}/UPowerProxy.h
	OUTPUT ${GENERATED_SOURCES}/UPowerProxy.cpp
	COMMAND qdbusxml2cpp -N -p UPowerProxy.h:UPowerProxy.cpp ${DBUS_XML}
	WORKING_DIRECTORY ${GENERATED_SOURCES}
)

cmake_policy(SET CMP0071 NEW) # Let AUTOMOC and AUTOUIC process GENERATED files.

# Executable output:
add_executable(
	${EXECUTABLE_FILE}
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/Proxy.h
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/Proxy.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/UPowerProxy.h
	${CMAKE_CURRENT_BINARY_DIR}/generated-sources/UPowerProxy.cpp
	MouseMainWindow.h
	MouseMainWindow.cpp
	cadMouseProGUI.cpp
)

# Link libraries:
target_link_libraries(${EXECUTABLE_FILE} Qt5::DBus)
target_link_libraries(${EXECUTABLE_FILE} Qt5::Widgets)
set_property(TARGET ${EXECUTABLE_FILE} PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
