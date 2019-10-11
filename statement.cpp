#include "statement.h"
#include "invoice.h"

namespace ride {
std::string statement(invoice invoice, int play) {
  auto totalAmount = 0;
  auto volumeCredits = 0;
  std::string result = "Statement for " + invoice.customer + '\n';
  char const format[] = "$%0.2f";
  return result;
}
}
