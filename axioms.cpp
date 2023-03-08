#include "theorem.h"
#include "axioms.h"


Axioms Axioms::copy () const {
	Axioms N;
	static int axioms_count = 0;
	N.m_id = ++ axioms_count;
	for (Theorem t : m_list)
		N.m_list.push_back(Theorem(t.m_pW, N.m_id));
	return N;
}
Axioms::Axioms (): m_id(0) {}
Axioms Axioms::pop (size_t pos) const {
	Axioms N = copy();
	N.m_list.erase(N.m_list.begin() + long(pos));
	return N;
}
Axioms Axioms::pop () const {
	Axioms N = copy();
	N.m_list.pop_back();
	return N;
}
Axioms Axioms::add (const Wff &W) const {
	Axioms N = copy();
	N.m_list.push_back(Theorem(&W, N.m_id));
	return N;
}
int Axioms::getId () const { return m_id; }
Theorem Axioms::getAxiom (size_t pos) const { return m_list[pos]; }
size_t Axioms::size () const { return m_list.size(); }
bool Axioms::checkId (Theorem t) const { return m_id == t.m_axiomsId; }
bool Axioms::operator == (Axioms A) const {
	if (m_list.size() != A.m_list.size()) return false;
	for (size_t i = 0; i < m_list.size(); i ++)
		if (m_list[i].Str() != A.m_list[i].Str())
			return false;
	return true;
}
