#include "associate_component.h"
using namespace rime;
an<Translation> AssociateTranslator::Query(const string& input,const Segment& segment) {
  return New<AssociateTranslation>();
}
void AssociateTranslator::Record() {} 
AssociateTranslator::~AssociateTranslator() {}

bool AssociateTranslation::Next() {
  return false;
}
bool AssociateSegmentor::Proceed(Segmentation* segmentation){
  return false;
}

an<Candidate> AssociateTranslation::Peek() { return New<SimpleCandidate>(); }