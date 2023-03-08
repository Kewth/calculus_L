#include <iostream>
#include "general.h"
#include "proof.h"
#include "wff.h"

ProofContent::ProofContent (size_t p1, size_t p2, WffSubstitution S, ProofContentType type, const Wff *pW): m_p1(p1), m_p2(p2), m_S(S), m_type(type), m_pW(pW) {}

size_t ProofContent::getP1 () const { return m_p1; }
size_t ProofContent::getP2 () const { return m_p2; }
const WffSubstitution & ProofContent::getSub () const { return m_S; }
ProofContentType ProofContent::getType () const { return m_type; }
const Wff & ProofContent::W () const { return *m_pW; }
void ProofContent::addOffset (size_t offset) {
	if (m_p1 != size_t(-1)) m_p1 += offset;
	if (m_p2 != size_t(-1)) m_p1 += offset;
}

PCIndex::PCIndex () {}
PCIndex::PCIndex (size_t i, Proof* p): m_i(i), m_proof(p) {}
PCIndex PCIndex::sub (WffSubstitution S) {
	m_proof->sub(m_i, S);
	return PCIndex(m_proof->getContents().size() - 1, m_proof);
}
const Wff & PCIndex::W () const { return m_proof->getContentWff(m_i); }
size_t PCIndex::getPos () const { return m_i; }
PCIndex PCIndex::checkW (const Wff &w) const { W().check(w); return *this; }

Proof::Proof (Axioms A, const Wff &W):
	m_axioms(A), m_pW(&W), m_contents(), m_steps(), m_step_level(0), m_done(false) {
		for (size_t i = 0; i < A.size(); i ++) {
			beginStep("Axiom");
			m_contents.push_back(ProofContent(
						size_t(-1), size_t(-1), WffSubstitution(), Content_AXIOM,
						&A.getAxiom(i)));
			endStep();
		}
	}

Proof & Proof::sub (size_t pos, WffSubstitution S) {
	beginStep("Subst");
	m_contents.push_back(ProofContent(pos, size_t(-1), S, Content_SUB,
				&m_contents[pos].W().substitute(S)));
	endStep();
	return *this;
}
Proof & Proof::MP (size_t p1, size_t p2) {
	beginStep("MonPe");
	m_contents.push_back(ProofContent(p1, p2, WffSubstitution(), Content_MP,
				&wffMP(m_contents[p1].W(), m_contents[p2].W())));
	endStep();
	return *this;
}

Proof & Proof::Import (const Proof &P) {
	beginStep("Impor");
	if (!(P.m_axioms <= m_axioms))
		Error("can not import");
	size_t offset = m_contents.size();
	for (ProofContent C : P.m_contents) {
		C.addOffset(offset);
		m_contents.push_back(C);
	}
	endStep();
	return *this;
}

Proof & Proof::Qed () {
	m_pW->check(m_contents.back().W());
	m_done = true;
	return *this;
}

Proof & Proof::Print () {
	for (auto pair : m_steps) {
		std::cout << pair.first << " " <<  pair.second << " "
			<< m_contents[pair.second].W().Str() << std::endl;
	}
	if (!m_done) {
		std::cout << "--------------" << std::endl;
		std::cout << m_pW->Str() << std::endl;
	}
	return *this;
}

Proof & Proof::PrintAll () {
	for (size_t i = 0; i < m_contents.size(); i ++) {
		switch (m_contents[i].getType()) {
			case Content_SUB: std::cout << "S "; break;
			case Content_MP: std::cout << "M "; break;
			case Content_AXIOM: std::cout << "A "; break;
		}
		std::cout << i << " ";
		std::cout << m_contents[i].W().Str() << std::endl;
	}
	return *this;
}

Proof & Proof::beginStep (string s) {
	if ((m_step_level ++) == 0)
		m_step_string = s;
	return *this;
}
Proof & Proof::endStep () {
	if ((-- m_step_level) == 0)
		m_steps.push_back(std::make_pair(m_step_string, m_contents.size() - 1));
	return *this;
}

const Wff & Proof::getAxiom (size_t pos) const { return m_axioms.getAxiom(pos); }
const Axioms & Proof::getAxioms () const { return m_axioms; }
const Wff & Proof::getContentWff (size_t i) const { return m_contents[i].W(); }
/* size_t Proof::numberOfContents () const { return m_contents.size(); } */
PCIndex Proof::getIndex (size_t i) { return PCIndex(i, (Proof*)this); }
PCIndex Proof::lastIndex () { return PCIndex(m_contents.size() - 1, (Proof*)this); }
const Wff & Proof::target () const { return *m_pW; }
const std::vector<ProofContent> & Proof::getContents () const { return m_contents; }

PCIndex MP (PCIndex I, PCIndex J) {
	if (I.m_proof != J.m_proof)
		Error("MP not in the same proof");
	I.m_proof->MP(I.m_i, J.m_i);
	return PCIndex(I.m_proof->getContents().size() - 1, I.m_proof);
}

/* Theorem Proof::Thm () const { */
/* 	if (!m_done) Error("not finished yet"); */
/* 	return m_contents.back(); */
/* } */
