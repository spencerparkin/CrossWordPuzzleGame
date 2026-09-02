# CMakeLists.txt file for the cross-word puzzle app.

set(CROSSWORD_PUZZLE_GAME_SOURCES
	Source/App.cpp
	Source/App.h
	Source/Frame.cpp
	Source/Frame.h
)

add_executable(CrossWordPuzzleGame WIN32
	${CROSSWORD_PUZZLE_GAME_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/wx_dpi_aware_pmv2.manifest
)

target_compile_definitions(CrossWordPuzzleGame PUBLIC
	__WXMSW__
	_UNICODE
)

target_compile_definitions(CrossWordPuzzleGame PRIVATE
	_USE_MATH_DEFINES
)

target_link_libraries(CrossWordPuzzleGame PRIVATE
	CrossWordPuzzleGenerator
	HappyMath
	FontSystem
	wx::core
	wx::base
	wx::gl
)