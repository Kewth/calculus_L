#include <iostream>
#include "general.h"
#include "theorem.h"
#include "proof.h"
#include "wff.h"

Proof::Proof (Axioms A, const Wff &W):
	m_axioms(A), m_pW(&W), m_history(), m_done(false) {}

Proof::Proof (Axioms A, const Wff &W, Proof P):
	m_axioms(A), m_pW(&W), m_history(), m_done(true) {
		if (!(m_axioms == P.m_axioms))
			Error("Different axioms");
		m_pW->check(*P.m_pW);
	}

Theorem Proof::getAxiom (size_t pos) const { return m_axioms.getAxiom(pos); }
Axioms Proof::getAxioms () const { return m_axioms; }
std::vector<Theorem> Proof::getHistory () const { return m_history; }

Proof & Proof::operator << (Theorem t) {
	if (m_done)
		Error("PROOF is finished");
	if (!m_axioms.checkId(t))
		Error("Add a wrong theorem");
	m_history.push_back(t);
	return *this;
}

Proof & Proof::operator << (ProofOperator e) {
	if (e == Qed) {
		if (m_history.empty())
			Error("EMPTY PROOF");
		m_history.back().check(*m_pW);
		m_done = true;
	}
	if (e == Pprint) {
		if (m_done)
			std::cout << "\033[32m" ">>>>> PROOF: " << m_pW->Str() << "\033[0m" << std::endl;
		else
			std::cout << "\033[31m" ">>>>> PROOF: " << m_pW->Str() << "\033[0m" << std::endl;
		for (size_t i = 0; i < m_axioms.size(); i ++)
			m_axioms.getAxiom(i).print();
		std::cout << "-----" << std::endl;
		for (Theorem t : m_history)
			t.print();
		if (m_done)
			std::cout << "<<<<< (done)" << std::endl;
		else
			std::cout << "<<<<< (to be continued)" << std::endl;
	}
	return *this;
}

/* Proof Proof::moveHypothesis (size_t pos) const { */
/* 	Theorem t = m_axioms.getAxiom(pos); */
/* 	if (!t.W().is_fixed()) Error("Move a hypothesis which is not fixed"); */
/* 	Proof P(m_axioms.pop(pos), t.W() >> *m_pW); */
/* 	P.m_done = true; */
/* 	return P; */
/* } */

Theorem Proof::Thm () const {
	if (!m_done) Error("not finished yet");
	return m_history.back();
}
