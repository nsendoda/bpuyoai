#ifndef BPUYOAI_FIN_H_
#define BPUYOAI_FIN_H_

#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include "field.h"
#include "types.h"

class Fin {
public:
  static Field StringToField(const std::string field_str);
  static const std::map<char, Color> color_map;
};
#endif
