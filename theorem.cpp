#include "theorem.h"
#include "wff.h"

Theorem::Theorem (const Wff *pW, int axiomsId):
	m_pW(pW), m_axiomsId(axiomsId) {}

Theorem::Theorem ():
	m_pW(nullptr), m_axiomsId(-1) {}

Theorem Theorem::sub (WffSubstitution S) const {
	return Theorem(&m_pW->substitute(S), m_axiomsId);
}

Theorem Theorem::print () const {
	m_pW->outputStr();
	return *this;
}

Theorem Theorem::check (const Wff &W) const {
	if (!m_pW) Error("EMPTY THEOREM!");
	m_pW->check(W);
	return *this;
}

string Theorem::Str () const { return m_pW->m_str; }

const Wff & Theorem::W () const { return *m_pW; }

Theorem MP (Theorem A, Theorem B) {
	if (A.m_axiomsId != B.m_axiomsId)
		Error("two theorem are not based on the same axioms.");
	return Theorem(&MP(*A.m_pW, *B.m_pW), A.m_axiomsId);
}

