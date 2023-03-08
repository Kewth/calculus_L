#include "axioms.h"
#include "wff.h"
using std::size_t;


Axioms::Axioms () {}
Axioms Axioms::pop (size_t pos) const {
	Axioms N = *this;
	N.m_list.erase(N.m_list.begin() + long(pos));
	return N;
}
Axioms Axioms::pop () const {
	Axioms N = *this;
	N.m_list.pop_back();
	return N;
}
Axioms Axioms::add (const Wff &W) const {
	Axioms N = *this;
	N.m_list.push_back(&W);
	return N;
}

bool Axioms::contains (const Wff &W) const {
	for (const Wff *p : m_list)
		if (p->same_as(W))
			return true;
	return false;
}

const Wff & Axioms::getAxiom (size_t pos) const { return *m_list[pos]; }
size_t Axioms::size () const { return m_list.size(); }
bool Axioms::operator <= (const Axioms &A) const {
	for (const Wff *p : m_list)
		if (!A.contains(*p))
			return false;
	return true;
}

bool Axioms::hasWffFix () const {
	for (const Wff *p : m_list)
		if (p->has_fixed())
			return true;
	return false;
}
