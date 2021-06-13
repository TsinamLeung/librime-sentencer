#include "phraser.h"

namespace rime {

  Phraser::Phraser(Config* config) {

  }

  Phraser::~Phraser() {}

  double Phraser::Query(const string& context,
    const string& word,
    bool is_rear) {
    printf("%s\t%s\t%d\t\n",context.c_str(),word.c_str(),is_rear);
    return 0.0;
  }
} // namespace rime