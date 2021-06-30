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
#include <functional>
#include <rime/key_event.h>
#include <rime/context.h>
#include <rime/dict/dictionary.h>
#include <queue>
#include "utf8.h"

namespace rime {
	class Engine;
	class Schema;
	class Config;
	class Memory;
	class Context;
	class Dictionary;

	// used to load dictionary	
	class AssociateMemory : public Memory {
	public:
		AssociateMemory(const Ticket& ticket) : Memory(ticket) {}

		virtual bool Memorize(const CommitEntry&);

		void SetMemorizeCallback(const std::function<void(const CommitEntry&)>& func) {
			this->callback_ = func;
		}

		virtual ~AssociateMemory() {};
	protected:
		std::function<void(const CommitEntry&)> callback_;
	};

	// use to store Global information for the associate Component
	class AssociateGlobal {
	public:
		AssociateGlobal() {};
		bool IsTrigger = false;
		string Prefix = "";
		KeyEvent hotkey;
		const string Namespace = "associate";
		an<AssociateMemory> Memory = nullptr;
	};


	// AssociateComponent Can inject the global varible to each Component
	template<typename T>
	class AssociateComponent : public T::Component {
	private:
		an<AssociateGlobal> env_;
	public:
		AssociateComponent(an< AssociateGlobal> env) : env_(env) {};
		T* Create(const Ticket& t) {
			Ticket a(t.engine, t.name_space, t.name_space);
			return new T(t, env_);
		}
	};

	class AssociateTranslator :
		public Translator {
	public:
		explicit AssociateTranslator(const Ticket& ticket, an<AssociateGlobal> env) : Translator(ticket), env(env) {
			LOG(INFO) << "Associate Component Namespace " << env->Namespace;

			if (this->engine_) {
				auto schema = this->engine_->schema();
				if (schema) {
					Config* config = schema->config();
					if (config) {
						config->GetString(this->env->Namespace + "/schema", &this->dictionary_name);
						config->GetInt(this->env->Namespace + "/result_limit", &this->result_limit);
						LOG(INFO) << "Loading associate Dictionary:[" << this->dictionary_name << "] with result limit:" << this->result_limit;

						Ticket customTicket = Ticket(this->engine_, "translator", "associate_translator");
						Schema schema(this->dictionary_name);
						customTicket.schema = &schema;

						this->env->Memory = New<AssociateMemory>(customTicket);

						// record the commited text
						Context* ctx = this->engine_->context();
						if (ctx) {
							auto f1 = std::bind(
								static_cast<void(AssociateTranslator::*)(Context*)>(&AssociateTranslator::Record),
								this, std::placeholders::_1);
							ctx->commit_notifier().connect(f1);
						}
					}
					else {
						LOG(ERROR) << "Config doesn't exist";
					}// end config
				}
				else {
					LOG(ERROR) << "Schema doesn't exist";
				}// end schema
			}
			else {
				LOG(ERROR) << "Engine doesn't exist";
			}//end engine_
		};
		virtual ~AssociateTranslator();
		virtual an<Translation> Query(const string& input,
			const Segment& segment);
		virtual void Record(Context*);
		virtual void Record(const string&);
	protected:
		string dictionary_name;
		int result_limit = 200;
		an<AssociateGlobal> env = nullptr;
	};

	class AssociateTranslation : public Translation {
	public:
		AssociateTranslation() { set_exhausted(true); };
		AssociateTranslation(const AssociateGlobal& global, int result_limit) {
			FetchWords(global.Prefix, global.Memory->dict() ,result_limit);
		}
		// A translation may contain multiple results, looks
		// something like a generator of candidates.
		virtual bool Next();

		virtual an<Candidate> Peek();

		void FetchWords(const string& search_prefix,Dictionary* dict, int result_limit);
	private:
		std::queue<pair<string,DictEntryIterator>> candidates;
	};

	//this segmentor use to tag the segment with specified tag that can be recevied by translator
	class AssociateSegmentor : public Segmentor {
	public:
		explicit AssociateSegmentor(const Ticket& ticket, an<AssociateGlobal> env)
			: Segmentor(ticket), env(env) {
			this->env = env;
		}

		virtual bool Proceed(Segmentation* segmentation);
	protected:
		an<AssociateGlobal> env;

	};

	class AssociateProcessor : public Processor {
	public:
		explicit AssociateProcessor(const Ticket& ticket, an< AssociateGlobal> env) : Processor(ticket), env(env) {
			if (this->engine_) {
				auto schema = this->engine_->schema();
				if (schema) {
					auto config = schema->config();
					if (config) {
						string hotkeyString = "";
						config->GetString(this->env->Namespace + "/hotkey", &hotkeyString);
						KeySequence k("{" + hotkeyString + "}");
						this->hotkey = k.front();
						if (this->env) {
							this->env->hotkey = this->hotkey;
						}
					}
				}
			}
		}

		virtual ProcessResult ProcessKeyEvent(const KeyEvent& key_event);
	protected:
		an< AssociateGlobal>env;
		KeyEvent hotkey;
	};

}

#endif // RIME_ASSOCIATE_TRANSLATOR_H