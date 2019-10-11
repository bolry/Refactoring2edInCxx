#pragma once
#include <string>
#include <vector>
namespace ride {

class performance{};

class invoice {
 public:
  std::string customer;
  std::vector<performance> performances;
};
}
