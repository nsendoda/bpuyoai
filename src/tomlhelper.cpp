#include "tomlhelper.h"

const std::map<std::string, RotateType> TomlHelper::toml_to_rotatetype = {
    {"ROTATE_0", RotateType::ROTATE_0},
    {"ROTATE_90", RotateType::ROTATE_90},
    {"ROTATE_180", RotateType::ROTATE_180},
    {"ROTATE_270", RotateType::ROTATE_270}};

const std::string CASEFIELD = "case.field", CASECOLUMN = "case.column",
                  CASEPUTTYPE_COLUMN = "case.puttype.column",
                  CASEPUTTYPE_ROTATE = "case.puttype.rotate",
                  CASENOWKUMIPUYO_PARENT = "case.now.parent",
                  CASENOWKUMIPUYO_CHILD = "case.now.child",
                  CASENEXTKUMIPUYO_PARENT = "case.next.parent",
                  CASENEXTKUMIPUYO_CHILD = "case.next.child",
                  CASECOLOR = "case.color";

const std::string ANSFIELD = "ans.field", ANSROW = "ans.row",
                  ANSSCORE = "ans.score",
                  ANSLOWESTEMPTYROWS = "ans.lowest_empty_rows",
                  ANSKUMIPUYOINDEX_PARENT = "ans.parent_index",
                  ANSKUMIPUYOINDEX_CHILD = "ans.child_index",
                  ANSVALIDPUTINDEXMAX = "ans.valid_putindex_max",
                  ANSVALIDPUTINDEXMIN = "ans.valid_putindex_min",
                  ANSINVALIDPUTINDEXES = "ans.invalid_putindexes",
                  ANSFRAME = "ans.frame", ANSPUTINDEXES = "ans.put_indexes",
                  ANSCHAINNUMBER = "ans.chain.number",
                  ANSCHAINSCORE = "ans.chain.score",
                  ANSCHAINFRAME = "ans.chain.frame";

std::vector<std::string> TomlHelper::GetTomlCasePaths(std::string folder) {
  std::string path = "./" + folder;
  std::regex reg(".*\\.toml");
  std::cout << "path = " << path << std::endl;
  char di[255];
  getcwd(di, 255);
  std::cout << "current directory:" << di << std::endl;
  std::vector<std::string> cases;

  DIR *dir;
  struct dirent *dent;
  dir = opendir(path.c_str());
  if (dir == NULL) {
    perror(path.c_str());
    return cases;
  }
  while ((dent = readdir(dir)) != NULL) {
    if (regex_match(dent->d_name, reg)) {
      cases.push_back(folder + dent->d_name);
    }
  }
  closedir(dir);
  return cases;
}

const toml::Value TomlHelper::Parse(std::string filename) {
  std::ifstream ifs(filename.c_str());
  toml::ParseResult pr = toml::parse(ifs);

  if (!pr.valid()) {
    cout << pr.errorReason << endl;
    return 0;
  }

  return pr.value;
}

Field TomlHelper::GetCaseField(const toml::Value &v) {
  return Fin::StringToField(v.get<std::string>(CASEFIELD));
}
Column TomlHelper::GetCaseColumn(const toml::Value &v) {
  return v.get<int>(CASECOLUMN);
}
Color TomlHelper::GetCaseColor(const toml::Value &v) {
  return Fin::color_map.at(v.get<std::string>(CASECOLOR)[0]);
}
PutType TomlHelper::GetCasePutType(const toml::Value &v) {
  return PutType(v.get<int>(CASEPUTTYPE_COLUMN),
                 toml_to_rotatetype.at(v.get<std::string>(CASEPUTTYPE_ROTATE)));
}
Kumipuyo TomlHelper::GetCaseNowKumipuyo(const toml::Value &v) {
  return Kumipuyo(
      Fin::color_map.at(v.get<std::string>(CASENOWKUMIPUYO_PARENT)[0]),
      Fin::color_map.at(v.get<std::string>(CASENOWKUMIPUYO_CHILD)[0]));
}

Kumipuyo TomlHelper::GetCaseNextKumipuyo(const toml::Value &v) {
  return Kumipuyo(
      Fin::color_map.at(v.get<std::string>(CASENEXTKUMIPUYO_PARENT)[0]),
      Fin::color_map.at(v.get<std::string>(CASENEXTKUMIPUYO_CHILD)[0]));
}

Field TomlHelper::GetAnsField(const toml::Value &v) {
  return Fin::StringToField(v.get<std::string>(ANSFIELD));
}
Row TomlHelper::GetAnsRow(const toml::Value &v) { return v.get<int>(ANSROW); }
Score TomlHelper::GetAnsScore(const toml::Value &v) {
  return v.get<int>(ANSSCORE);
}
std::vector<int> TomlHelper::GetAnsLowestEmptyRows(const toml::Value &v) {
  return v.get<std::vector<int>>(ANSLOWESTEMPTYROWS);
}
std::pair<FieldIndex, FieldIndex>
TomlHelper::GetAnsKumipuyoIndex(const toml::Value &v) {
  return {v.get<int>(ANSKUMIPUYOINDEX_PARENT),
          v.get<int>(ANSKUMIPUYOINDEX_CHILD)};
}

int TomlHelper::GetAnsValidPutIndexMin(const toml::Value &v) {
  return v.get<int>(ANSVALIDPUTINDEXMIN);
}
int TomlHelper::GetAnsValidPutIndexMax(const toml::Value &v) {
  return v.get<int>(ANSVALIDPUTINDEXMAX);
}
std::vector<int> TomlHelper::GetAnsInvalidPutIndexes(const toml::Value &v) {
  return v.get<std::vector<int>>(ANSINVALIDPUTINDEXES);
}
Frame TomlHelper::GetAnsFrame(const toml::Value &v) {
  return v.get<int>(ANSFRAME);
}
Chain TomlHelper::GetAnsChain(const toml::Value &v) {
  return Chain(v.get<int>(ANSCHAINNUMBER), v.get<int>(ANSCHAINSCORE),
               v.get<int>(ANSCHAINFRAME));
}
std::vector<PutIndex> TomlHelper::GetAnsPutIndexes(const toml::Value &v) {
  return v.get<std::vector<int>>(ANSPUTINDEXES);
}
