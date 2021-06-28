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

namespace rime {
	class Engine;
	class Schema;
	class Config;
	class Memory;

	// use to store Global information for the associate Component
	class AssociateGlobal {
	public:
		AssociateGlobal() {};

	};


	// AssociateComponent Can inject the global varible to each Component
	template<typename T>
	class AssociateComponent : public T::Component {
	private:
		an<AssociateGlobal> env_;
	public:
		AssociateComponent(an< AssociateGlobal> env) : env_(env) {};
		T* Create(const Ticket &t) {
			Ticket a(t.engine,t.name_space,t.name_space);
			return new T(t,env_);
		}
	};


	class AssociateMemory : public Memory {
	public:
		AssociateMemory(const Ticket& ticket) : Memory(ticket) {
			
		}
		virtual bool Memorize(const CommitEntry&);

		void SetMemorizeCallback(const std::function<void(const CommitEntry&)>& func) {
			this->callback_ = func;
		}

		virtual ~AssociateMemory() {};
	protected:
		std::function<void(const CommitEntry&)> callback_;
	};

	class AssociateTranslator :
		public Translator {
	public:
		explicit AssociateTranslator( const Ticket& ticket,an<AssociateGlobal> env) : Translator(ticket) {

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
						Ticket customTicket = Ticket(this->engine_, "translator", "associate_translator");
						Schema schema(this->dictionary_name);
						customTicket.schema = &schema;

						this->memory = New<AssociateMemory>(customTicket);
						this->env = env;
						this->memory->SetMemorizeCallback(std::bind(&AssociateTranslator::callbackTest,this,std::placeholders::_1));
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
		virtual void Record();

	protected:
		void callbackTest(const CommitEntry& entry) {
			printf("sAY FUCK THE WORLD\n");
		}
		string dictionary_name;
		int result_limit = 200;
		an<AssociateMemory> memory = nullptr;
		an<AssociateGlobal> env = nullptr;
	};

	class AssociateTranslation : public Translation {
	public:
		AssociateTranslation() {};

		// A translation may contain multiple results, looks
		// something like a generator of candidates.
		virtual bool Next();

		virtual an<Candidate> Peek();
	};

	class AssociateSegmentor : public Segmentor {
	public:
		explicit AssociateSegmentor(const Ticket& ticket,an<AssociateGlobal> env)
			: Segmentor(ticket) {
			this->env = env;
		}

		virtual bool Proceed(Segmentation* segmentation);
	protected:
		an<AssociateGlobal> env;
	};

	class AssociateProcessor : public Processor {
	public:
		explicit AssociateProcessor( const Ticket& ticket,an< AssociateGlobal> env) : Processor(ticket) {
			this->env = env;
		}
	protected:
		an< AssociateGlobal>env;
	};

}

#endif // RIME_ASSOCIATE_TRANSLATOR_H