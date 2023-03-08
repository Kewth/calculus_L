#ifndef PROOF_H
#define PROOF_H
#include <vector>
#include "axioms.h"
#include "wff.h"

enum ProofContentType { Content_SUB, Content_MP, Content_AXIOM };
class ProofContent {
	size_t m_p1, m_p2;
	WffSubstitution m_S;
	ProofContentType m_type;
	const Wff *m_pW;
public:
	ProofContent (size_t, size_t, WffSubstitution, ProofContentType, const Wff *);
	size_t getP1 () const;
	size_t getP2 () const;
	const WffSubstitution & getSub () const;
	ProofContentType getType () const;
	const Wff & W () const;
	void addOffset (size_t);
};

class Proof;
class PCIndex {
	size_t m_i;
	Proof *m_proof;
public:
	PCIndex ();
	PCIndex (size_t, Proof*);
	PCIndex sub (WffSubstitution);
	const Wff & W () const;
	size_t getPos () const;
	PCIndex checkW (const Wff &) const;
	friend PCIndex MP (PCIndex, PCIndex);
};

class Proof {
	Axioms m_axioms;
	const Wff *m_pW; // target
	std::vector<ProofContent> m_contents;
	bool m_done;
public:
	Proof (Axioms, const Wff &);
	/* Proof (Axioms, const Wff &, Proof); */
	Proof & sub (size_t, WffSubstitution);
	Proof & MP (size_t, size_t);
	Proof & Import (const Proof &);
	Proof & Qed ();
	Proof & Print ();
	const Wff & getAxiom (size_t) const;
	const Axioms & getAxioms () const;
	const Wff & getContentWff (size_t) const;
	/* size_t numberOfContents () const; */
	PCIndex getIndex (size_t);
	PCIndex lastIndex ();
	const Wff & target () const;
	const std::vector<ProofContent> & getContents () const;
};

PCIndex MP (PCIndex, PCIndex);

#endif
