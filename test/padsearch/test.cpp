/*
 * main.cpp
 * */

#include <gtest/gtest.h>

#include "chain.h"
#include "field.h"
#include "fin.h"
#include "kumipuyo.h"
#include "search.h"
#include "search_until_next.h"
#include "tomlhelper.h"

std::vector<std::string> GetTomls(std::string folder) {
  std::vector<std::string> tomls =
      TomlHelper::GetTomlCasePaths("../../../test/padsearch/" + folder);
  for (auto s : tomls) {
    std::cout << s << std::endl;
  }
  std::cout << tomls.size() << std::endl;
  return tomls;
}

// first parameter is testcase name, second parameter is test name
TEST(SearchUntilNext, all) {
  auto tomls = GetTomls("droporder/");
  for (std::string toml : tomls) {
    toml::Value v = TomlHelper::Parse(toml);
    // case
    Field case_field = TomlHelper::GetCaseField(v);
    Kumipuyo case_now = TomlHelper::GetCaseNowKumipuyo(v);
    Kumipuyo case_next = TomlHelper::GetCaseNextKumipuyo(v);
    State case_state(case_field, case_now, case_next);
    // ans
    std::vector<PutIndex> ans_put_indexes(TomlHelper::GetAnsPutIndexes(v));
    Chain ans_chain(TomlHelper::GetAnsChain(v));

    // Search
    SearchResultQueue queue = SearchUntilNext::Excute(case_state, 4200);

    if (queue.Empty()) {
      EXPECT_TRUE(ans_put_indexes.size() == 0);
    } else {
      std::vector<PutIndex> actual_put_indexes(queue.TopPutIndexes());
      Chain actual_chain(queue.TopChain());

      std::cout << actual_put_indexes[0] << std::endl;

      EXPECT_EQ(ans_put_indexes, actual_put_indexes);
      EXPECT_EQ(ans_chain.number, actual_chain.number);
      EXPECT_EQ(ans_chain.score, actual_chain.score);
      EXPECT_EQ(ans_chain.frame, actual_chain.frame);
    }
  }
}
