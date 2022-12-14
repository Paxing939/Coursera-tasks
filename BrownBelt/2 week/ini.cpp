//
// Created by ilya on 14.12.22.
//

#include "ini.h"

#include <vector>

namespace Ini {

  std::vector<std::string> SplitIntoWords(std::string str, const std::string &delimiter) {
    size_t pos;
    std::vector<std::string> result;
    while ((pos = str.find(delimiter)) != std::string::npos) {
      result.push_back(str.substr(0, pos));
      str = str.substr(pos + delimiter.size());
    }
    result.emplace_back(str);
    return result;
  }

  Section &Document::AddSection(std::string name) {
    return sections[name];
  }

  const Section &Document::GetSection(const std::string &name) const {
    return sections.at(name);
  }

  size_t Document::SectionCount() const {
    return sections.size();
  }

  Document Load(std::istream &input) {
    Document document;

    std::string line;
    Section section;
    while (std::getline(input, line)) {
      if (line[0] == '[' && line.back() == ']') {
        section = document.AddSection(line.substr(1, int(line.size()) - 2));
      } else if (!line.empty()) {
        std::vector<std::string> divided_line = SplitIntoWords(line, "=");
        section[divided_line[0]] = divided_line[1];
      }
    }

    return document;
  }

};
