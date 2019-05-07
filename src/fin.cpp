#include "fin.h"

const std::map<char, Color> Fin::color_map = {
    {'O', EMPTY},  {'o', EMPTY},  {'R', RED},   {'r', RED},
    {'Y', YELLOW}, {'y', YELLOW}, {'g', GREEN}, {'G', GREEN},
    {'P', PURPLE}, {'p', PURPLE}, {'B', BLUE},  {'b', BLUE},
    {'X', OJAMA},  {'x', OJAMA},  {'W', WALL},  {'w', WALL}};

Field Fin::StringToField(const std::string field_str) {
  std::string temp_row;
  std::vector<std::string> rows;
  std::string arranged_field_str;
  for (auto c : field_str) {
    if (std::isalnum(c)) {
      temp_row += c;
    } else if (!temp_row.empty()) {
      rows.push_back(temp_row);
      temp_row = "";
    }
  }
  if (!temp_row.empty()) {
    rows.push_back(temp_row);
    temp_row = "";
  }
  std::reverse(rows.begin(), rows.end());
  for (auto str : rows) {
    arranged_field_str += str;
  }
  Field field;
  if (arranged_field_str.size() != Field::FIELD_SIZE) {
    std::cout << "toml field is invalid size. expected: " << Field::FIELD_SIZE
              << " actual: " << arranged_field_str.size() << std::endl;
    std::cout << arranged_field_str << std::endl;
  }
  for (int i = 0; i < arranged_field_str.size(); i++) {
    field[i] = color_map.at(arranged_field_str[i]);
  }
  field.ModifyLowestEmptyRows();
  return field;
}
