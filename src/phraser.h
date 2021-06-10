#ifndef RIME_PHRASER_H_
#define RIME_PHRASER_H_

#include <rime/common.h>
#include <rime/component.h>
#include <rime/resource.h>
#include <rime/gear/grammar.h>

namespace rime {

class Phraser : public Grammar {
 public:
  explicit Phraser(Config* config);
  virtual ~Phraser();
  double Query(const string& context,
               const string& word,
               bool is_rear) override;
 private:
};

}  // namespace rime
#endif  // RIME_PHRASER_H_