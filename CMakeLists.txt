project( QNotepad )

# CMake 3.* necessario per Qt
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# ottengo tutti i sorgenti
file(GLOB my_sources *.h *.cpp *.qrc)

# attiva il Resource Compiler (RCC)
set(CMAKE_AUTORCC ON)
# attiva il Meta-Object Compiler
set(CMAKE_AUTOMOC ON)

# aggiungo eseguibile
add_executable(QNotepad ${my_sources})

# verifico l'esistenza delle librerie Qt necessarie
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# collego le librerie Qt con l'eseguibile
target_link_libraries(QNotepad Qt5::Widgets Qt5::PrintSupport)
