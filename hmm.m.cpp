#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

#include <json/json.h>

namespace {

std::size_t constexpr mix(char m, std::size_t s) {
  return ((s << 7) + ~(s >> 3)) + ~m;
}

std::size_t constexpr fastHash(char const *m) {
  return (*m) ? mix(*m, fastHash(m + 1)) : 0u;
}

std::ostringstream statement(Json::Value const &invoice,
                             Json::Value const &plays) {
  int totalAmount{0};
  int volumeCredits{0};
  std::ostringstream result{"Statement for " + invoice["customer"].asString() +
                                '\n',
                            std::ios_base::ate};
  auto format = [](std::ostream &os, int cents) -> std::ostream & {
    auto const oldLocale = os.imbue(std::locale("en_US.UTF-8"));
    auto const oldFlags = os.setf(std::ios::showbase);
    os << std::put_money(cents);
    os.setf(oldFlags, std::ios::showbase);
    os.imbue(oldLocale);
    return os;
  };
  for (auto const &perf : invoice["performances"]) {
    auto const play{plays[perf["playID"].asString()]};
    int thisAmount{0};

    switch (fastHash(play["type"].asCString())) {
    case fastHash("tragedy"):
      thisAmount = 40'000;
      if (perf["audience"].asInt() > 30) {
        thisAmount += 1'000 * (perf["audience"].asInt() - 30);
      }
      break;
    case fastHash("comedy"):
      thisAmount = 30'000;
      if (perf["audience"].asInt() > 20) {
        thisAmount += 10'000 + 500 * (perf["audience"].asInt() - 20);
      }
      thisAmount += 300 * perf["audience"].asInt();
      break;
    default:
      throw std::runtime_error("unknown type: " + play["type"].asString());
    }

    // add volume credit
    volumeCredits += std::max(perf["audience"].asInt() - 30, 0);
    // add extra credit for every ten comedy attendees
    if ("comedy" == play["type"].asString())
      volumeCredits += perf["audience"].asInt() / 5;

    // print line for this order
    result << "  " << play["name"].asString() << ' ';
    format(result, thisAmount)
        << " (" << perf["audience"].asString() << " seats)\n";
    totalAmount += thisAmount;
  }
  result << "Amount owed is ";
  format(result, totalAmount) << '\n'
                              << "You earned " << volumeCredits << " credits\n";
  return result;
}

} // namespace

int main() {
  std::ifstream ifsPlays{"../data/plays.json"};
  std::ifstream ifsInvoices{"../data/invoices.json"};
  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  Json::Value plays;
  JSONCPP_STRING playsErrs;
  bool const playsOk =
      Json::parseFromStream(builder, ifsPlays, &plays, &playsErrs);
  Json::Value invoices;
  JSONCPP_STRING invoicesErrs;
  bool const invoicesOk =
      Json::parseFromStream(builder, ifsInvoices, &invoices, &invoicesErrs);
  if (playsOk && invoicesOk) {
    for (auto const &invoice : invoices) {
      std::cout << statement(invoice, plays).str();
    }
  } else {
    if (!playsOk) {
      std::cout << "Unable to read plays due to:\n";
      std::cout << playsErrs << '\n';
    }
    if (!invoicesOk) {
      std::cout << "Unable to read invoices due to:\n";
      std::cout << invoicesErrs << '\n';
    }
  }
  std::cout.flush();
  return EXIT_SUCCESS;
}
