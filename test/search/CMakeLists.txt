cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(TestSearch test.cpp)
target_link_libraries(TestSearch BpuyoAI GTest::GTest GTest::Main)
include_directories(${PROJECT_SOURCE_DIR}/src ${GTEST_INCLUDE_DIRS})

# Google Testの各テストケースごとにCTestのテストを作成する
gtest_add_tests(TARGET TestSearch)

# CTestのテストをひとつだけ作成する
#add_test(NAME AllTests COMMAND TestSample1)
