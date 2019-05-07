#ifndef BPUYOAI_TOMLHELPER_H_
#define BPUYOAI_TOMLHELPER_H_
#include "chain.h"
#include "fin.h"
#include "kumipuyo.h"
#include "puttype.h"
#include "toml.h"
#include "types.h"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>

using namespace std;

class TomlHelper {
private:
  static const std::vector<std::string> TOINT_NAMES;
  static const std::vector<std::string> TOSTRING_NAMES;
  static const std::vector<std::string> TOVECTOR_NAMES;
  static const std::map<std::string, RotateType> toml_to_rotatetype;

public:
  static std::vector<std::string> GetTomlCasePaths(std::string folder);
  static const toml::Value Parse(std::string filename);

  static Field GetCaseField(const toml::Value &v);
  static Column GetCaseColumn(const toml::Value &v);
  static Color GetCaseColor(const toml::Value &v);
  static PutType GetCasePutType(const toml::Value &v);
  static Kumipuyo GetCaseNowKumipuyo(const toml::Value &v);
  static Kumipuyo GetCaseNextKumipuyo(const toml::Value &v);

  static Field GetAnsField(const toml::Value &v);
  static Row GetAnsRow(const toml::Value &v);
  static Score GetAnsScore(const toml::Value &v);
  static std::vector<int> GetAnsLowestEmptyRows(const toml::Value &v);
  static std::pair<FieldIndex, FieldIndex>
  GetAnsKumipuyoIndex(const toml::Value &v);
  static int GetAnsValidPutIndexMin(const toml::Value &v);
  static int GetAnsValidPutIndexMax(const toml::Value &v);
  static std::vector<int> GetAnsInvalidPutIndexes(const toml::Value &v);
  static Frame GetAnsFrame(const toml::Value &v);
  static Chain GetAnsChain(const toml::Value &v);
  static std::vector<PutIndex> GetAnsPutIndexes(const toml::Value &v);
};
#endif
