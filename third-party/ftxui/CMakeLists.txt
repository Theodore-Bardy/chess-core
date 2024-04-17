# @file  CMakeLists.txt
# @brief CMakeLists file for FTXUI library

# Include FTXUI library
set(GIT_REPO_URL "https://github.com/ArthurSonzogni/FTXUI.git")
set(GIT_TAG_NAME "v5.0.0")

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY  ${GIT_REPO_URL}
    GIT_TAG         ${GIT_TAG_NAME}
)
FetchContent_MakeAvailable(ftxui)

add_executable(ftxui-starter src/ftxui_demo.cpp)
target_link_libraries(ftxui-starter
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)
