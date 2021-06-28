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
	
  an<AssociateGlobal> associateGlobal = New<AssociateGlobal>();
  r.Register("associate_processor", new AssociateComponent<AssociateProcessor>(associateGlobal));
  r.Register("associate_translator", new AssociateComponent<AssociateTranslator>(associateGlobal));
  r.Register("associate_segmentor", new AssociateComponent<AssociateSegmentor>(associateGlobal));
}

static void rime_grammar_finalize() {}

RIME_REGISTER_MODULE(grammar)