/*
 * main.cpp
 * */

#include <gtest/gtest.h>

#include "chain.h"
#include "field.h"
#include "fin.h"
#include "kumipuyo.h"
#include "simulator.h"
#include "tomlhelper.h"

std::vector<std::string> GetTomls(std::string folder) {
  std::vector<std::string> tomls =
      TomlHelper::GetTomlCasePaths("../../../test/simulator/" + folder);
  for (auto s : tomls) {
    std::cout << s << std::endl;
  }
  std::cout << tomls.size() << std::endl;
  return tomls;
}

bool EqualsLowestEmptyRowIndexes(Field &field, std::vector<int> &l) {
  for (int i = 1; i < Field::COLUMN; i++) {
    if (field.GetLowestEmptyRows(i) != l[i])
      return false;
  }
  return true;
}

// first parameter is testcase name, second parameter is test name
TEST(Simulate, all) {
  auto tomls = GetTomls("chain/");
  for (std::string toml : tomls) {
    toml::Value v = TomlHelper::Parse(toml);
    Field case_field = TomlHelper::GetCaseField(v);
    Chain score = Simulator::Simulate(&case_field);
    EXPECT_EQ(v.get<int>("ans.score"), score.score) << v.get<int>("ans.score")
                                                    << " " << score.score;
  }
}

TEST(Fall, all) {
  auto tomls = GetTomls("fall/");
  for (std::string toml : tomls) {
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    // expected
    Field ans_field = TomlHelper::GetAnsField(v);
    Row ans_row = TomlHelper::GetAnsRow(v);
    auto ans_lowest_emtpy_rows = TomlHelper::GetAnsLowestEmptyRows(v);
    std::cout << "case_field:" << std::endl;
    case_field.OutputIncludeWall();
    // FALL
    Row actual_row = Simulator::FallAll(&case_field);

    std::cout << "falled case_field:" << std::endl;
    case_field.OutputIncludeWall();
    std::cout << "ans_field:" << std::endl;
    ans_field.OutputIncludeWall();
    std::cout << "actual row: " << actual_row << " ans_row: " << ans_row
              << std::endl;
    EXPECT_TRUE(EqualsLowestEmptyRowIndexes(case_field, ans_lowest_emtpy_rows));
    EXPECT_TRUE(case_field.Equals(ans_field));
    EXPECT_EQ(ans_row, actual_row);
  }
}

TEST(Put, all) {
  auto tomls = GetTomls("put/");
  for (std::string toml : tomls) {
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    Kumipuyo case_kumipuyo = TomlHelper::GetCaseNowKumipuyo(v);
    PutType case_puttype = TomlHelper::GetCasePutType(v);

    // Put
    Frame actual_frame =
        Simulator::Put(case_kumipuyo, &case_field, case_puttype);

    // ans
    Field ans_field = TomlHelper::GetAnsField(v);
    Frame ans_frame = TomlHelper::GetAnsFrame(v);
    std::vector<int> ans_lowest_emtpy_rows =
        TomlHelper::GetAnsLowestEmptyRows(v);

    case_field.OutputIncludeWall();
    ans_field.OutputIncludeWall();
    // TEST
    EXPECT_TRUE(case_field.Equals(ans_field));
    EXPECT_TRUE(EqualsLowestEmptyRowIndexes(case_field, ans_lowest_emtpy_rows));
    //    EXPECT_EQ(ans_frame, actual_frame);
  }
}

TEST(PutOnePuyo, all) {
  auto tomls = GetTomls("addpuyo/");
  for (std::string toml : tomls) {
    std::cout << "testcase name: " << toml << std::endl;
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    Column case_column = TomlHelper::GetCaseColumn(v);
    Color case_color = TomlHelper::GetCaseColor(v);

    // PutOnePuyo
    FieldIndex actual_index =
        Simulator::PutOnePuyo(&case_field, case_column, case_color);

    // ans
    Field ans_field = TomlHelper::GetAnsField(v);
    std::vector<int> ans_lowest_emtpy_rows =
        TomlHelper::GetAnsLowestEmptyRows(v);

    // TEST
    EXPECT_TRUE(case_field.Equals(ans_field));
    EXPECT_TRUE(EqualsLowestEmptyRowIndexes(case_field, ans_lowest_emtpy_rows));
  }
}

TEST(EliminateOnePuyo, all) {
  auto tomls = GetTomls("eliminate/");
  for (std::string toml : tomls) {
    std::cout << "testcase name: " << toml << std::endl;
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    Column case_column = TomlHelper::GetCaseColumn(v);

    // Eliminate
    Simulator::EliminateOnePuyo(&case_field, case_column);

    // ans
    Field ans_field = TomlHelper::GetAnsField(v);
    std::vector<int> ans_lowest_emtpy_rows =
        TomlHelper::GetAnsLowestEmptyRows(v);

    // TEST
    EXPECT_TRUE(case_field.Equals(ans_field));
    EXPECT_TRUE(EqualsLowestEmptyRowIndexes(case_field, ans_lowest_emtpy_rows));
  }
}

TEST(CalculatePutIndex, all) {
  auto tomls = GetTomls("calculateput/");
  for (std::string toml : tomls) {
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    PutType case_puttype = TomlHelper::GetCasePutType(v);

    // Put
    std::pair<FieldIndex, FieldIndex> actual_kumipuyoindex =
        Simulator::CalculatePutIndex(case_puttype, case_field);

    // ans
    std::pair<FieldIndex, FieldIndex> ans_kumipuyoindex =
        TomlHelper::GetAnsKumipuyoIndex(v);

    // TEST
    EXPECT_TRUE(actual_kumipuyoindex == ans_kumipuyoindex);
    //    EXPECT_EQ(ans_frame, actual_frame);
  }
}

TEST(CanPut, all) {
  auto tomls = GetTomls("canput/");
  for (std::string toml : tomls) {
    std::cout << "testcase name: " << toml << std::endl;
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    PutType case_put;
    // ans
    int ans_valid_putindex_min = TomlHelper::GetAnsValidPutIndexMin(v);
    int ans_valid_putindex_max = TomlHelper::GetAnsValidPutIndexMax(v);
    std::vector<int> ans_invalid_putindexes =
        TomlHelper::GetAnsInvalidPutIndexes(v);
    std::cout << "ans (min, max) = " << ans_valid_putindex_min << ' '
              << ans_valid_putindex_max << std::endl;
    for (int puttype = 0; puttype < PUTTYPE_PATTERN; puttype++) {
      std::cout << "puttype: " << puttype << std::endl;
      case_put.SetPutType(puttype);
      bool actual_canput = Simulator::CanPut(case_put, case_field);
      bool is_puttype_validrange = (ans_valid_putindex_min <= puttype &&
                                    ans_valid_putindex_max >= puttype);

      bool catched_invalid_putindexes = false;
      for (int invalid_putindex : ans_invalid_putindexes) {
        if (invalid_putindex == puttype) {
          catched_invalid_putindexes = true;
          EXPECT_FALSE(actual_canput);
          break;
        }
      }
      if (!catched_invalid_putindexes)
        EXPECT_EQ(actual_canput, is_puttype_validrange);
    }
  }
}
