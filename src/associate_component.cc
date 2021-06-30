#include "associate_component.h"
using namespace rime;
an<Translation> AssociateTranslator::Query(const string& input, const Segment& segment) {
	if (segment.HasTag("associate") && this->env->Memory) {
		// action could be done in Translation
		return New<AssociateTranslation>(*this->env, this->result_limit);
	}
	return New<AssociateTranslation>();
}
void AssociateTranslator::Record(Context* ctx) {
	if (!ctx) return;
	this->env->Prefix = ctx->GetCommitText();
}
void AssociateTranslator::Record(const string& input) { this->env->Prefix = input; }

AssociateTranslator::~AssociateTranslator() {}

bool AssociateTranslation::Next() {
	while (!candidates.empty()) {
		auto iter = candidates.front().second;
		if (!iter.Next())
		{
			candidates.pop();
			continue;
		}
		return true;
	}
	set_exhausted(true);
	return false;
}
// let let space input tag as "associate"
bool AssociateSegmentor::Proceed(Segmentation* segmentation) {
	if (this->env->IsTrigger) {
		this->env->IsTrigger = false;
		auto seg = Segment(0, 1); // input: " "
		seg.tags.insert("associate"); // insert tag
		segmentation->Forward();
		segmentation->AddSegment(seg);
		return false;
	}
	return true;
}

an<Candidate> AssociateTranslation::Peek() {
	if (candidates.empty())
		return nullptr;
	auto candidate = candidates.front();
	auto& prefix = candidate.first;
	an<DictEntry>& result = candidate.second.Peek();
	if (!result)
	{
		candidates.pop();
		return nullptr;
	}
	return New<SimpleCandidate>("associate", 0, 1, result->text.substr(prefix.size()), "");
}

void rime::AssociateTranslation::FetchWords(const string& prefix, Dictionary* dict, int result_limit)
{
	if (result_limit <= 0 || !prefix.size())
	{
		return;
	}
	if (!dict) {
		LOG(ERROR) << "Cannot find dictionary";
		return;
	}

	DictEntryIterator iter;
	dict->LookupWords(&iter, prefix, true, result_limit);
	// find nothing
	if (!iter.entry_count()) {
		// exhaust the prefix
		return FetchWords(prefix.substr(1), dict, result_limit);;
	}

	iter.Next(); // the first result are itself usually.
	if (iter.exhausted()) {
		return;
	}

	result_limit -= iter.entry_count() - 1;
	candidates.push(make_pair(prefix, iter));

	//fuck theutf8
	return FetchWords(prefix.substr(1),dict,result_limit);
}




bool rime::AssociateMemory::Memorize(const CommitEntry& entry) {
	this->callback_(entry);
	return true;
}

ProcessResult rime::AssociateProcessor::ProcessKeyEvent(const KeyEvent& key_event)
{
	if (this->hotkey == key_event) {
		this->env->IsTrigger = true;
		this->engine_->context()->set_input(" ");
		return kAccepted;
	}
	return kNoop;
}
