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
#include <rime/gear/memory.h>

namespace rime {
class Engine;
class Schema;
class Config;
class Memory;

class AssociateTranslator : 
  public Translator {
public:
  explicit AssociateTranslator(const Ticket& ticket) : Translator(ticket) {
   
    if (ticket.name_space == "translator") {
      LOG(INFO) << "Setting namespace to associate";
      this->name_space_ = "associate";
    }
    if (this->engine_) {
      auto schema = this->engine_->schema();
      if (schema) {
        auto config = schema->config();
        if (config) {
          config->GetString(this->name_space_ + "/dictionary", &this->dictionary_name);
          config->GetInt(this->name_space_ + "/result_limit", &this->result_limit);
          LOG(INFO) << "Loading associate Dictionary:[" << this->dictionary_name << "] with result limit:" << this->result_limit;
          this->memory_ = New<Memory>(Ticket(this->engine_,this->dictionary_name,"")); 
          printf("testing %s %s", this->name_space_.c_str(), dictionary_name.c_str());
        }
        else {
          LOG(ERROR) << "Config doesn't exist";
        }// end config
      }else{
        LOG(ERROR) << "Schema doesn't exist";
      }// end schema
    }else {
      LOG(ERROR) << "Engine doesn't exist";
    }//end engine_
  };
  virtual ~AssociateTranslator();
  virtual an<Translation> Query(const string& input,
                                const Segment& segment);
  virtual void Record(); 

protected:
  string dictionary_name;
  int result_limit = 200;
  an<Memory> memory_ = nullptr;
};

class AssociateTranslation : public Translation {
  public:
    AssociateTranslation(){};

   // A translation may contain multiple results, looks
   // something like a generator of candidates.
   virtual bool Next();

   virtual an<Candidate> Peek();
};

class AssociateSegmentor : public Segmentor {
public:
  explicit AssociateSegmentor(const Ticket& ticket)
      : Segmentor(ticket) {}

  virtual bool Proceed(Segmentation* segmentation);

};

class AssociateProcessor : public Processor {
 public:
  explicit AssociateProcessor(const Ticket& ticket) : Processor(ticket) {}
};

}

#endif // RIME_ASSOCIATE_TRANSLATOR_H