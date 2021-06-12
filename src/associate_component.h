#ifndef RIME_ASSOCIATE_TRANSLATOR_H_
#define RIME_ASSOCIATE_TRANSLATOR_H_
#pragma once

#include <rime/common.h>
#include <rime/translation.h>
#include <rime/translator.h>
#include <rime/segmentor.h>
#include <rime/processor.h>
#include <rime/gear/translator_commons.h>
#include <rime/engine.h>
#include <rime_api.h>
#include <rime/schema.h>

namespace rime {
class Engine;
class Schema;
class Config;
class AssociateTranslator : 
  public Translator {
public:
  explicit AssociateTranslator(const Ticket& ticket) : Translator(ticket) {
    if (this->engine_) {
      auto schema = this->engine_->schema();
      if (schema) {
        schema->config;
      }
    }
  };
  virtual ~AssociateTranslator();
  virtual an<Translation> Query(const string& input,
                                const Segment& segment);

};

class AssociateTranslation :
  public Translation {

};

class AssociateSegmentor :
  public Processor {

};

class AssociateProcessor {

};

}

#endif // RIME_ASSOCIATE_TRANSLATOR_H