//
// Copyright RIME Developers
// Distributed under GPLv3
//

#include "phraser.h"
#include "associate_component.h"
#include <rime/common.h>
#include <rime/registry.h>
#include <rime_api.h>

static void rime_grammar_initialize() {
  using namespace rime;

  LOG(INFO) << "registering components from module 'grammar'.";
  Registry& r = Registry::instance();
  r.Register("grammar", new Component<Phraser>);
  r.Register("associate_processor", new Component<AssociateProcessor>);
  r.Register("associate_translator", new Component<AssociateTranslator>);
  r.Register("associate_segmentor", new Component<AssociateSegmentor>);
}

static void rime_grammar_finalize() {}

RIME_REGISTER_MODULE(grammar)