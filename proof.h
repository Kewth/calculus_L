#ifndef PROOF_H
#define PROOF_H
#include <vector>
#include "axioms.h"
#include "wff.h"

enum ProofContentType { Content_SUB, Content_SUBFIX, Content_MP, Content_AXIOM };
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
	// TODO: this function is dangeraous,
	// it should be deleted when we delete "Proof::Import".
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
	PCIndex subFix (WffSubstitution);
	const Wff & W () const;
	size_t getPos () const;
	PCIndex checkW (const Wff &) const;
	friend PCIndex MP (PCIndex, PCIndex);
};

class ProofSteps {
public:
};

class Proof {
	Axioms m_axioms;
	const Wff *m_pW; // target
	std::vector<ProofContent> m_contents;
	std::vector<std::pair<string, size_t> > m_steps;
	string m_step_string;
	int m_step_level;
	bool m_done;
public:
	Proof (Axioms, const Wff &);
	Proof & sub (size_t, WffSubstitution);
	Proof & subFix (size_t, WffSubstitution);
	Proof & MP (size_t, size_t);
	// TODO: "Import" is not an AXIOM so
	// it can be implemented by user in principle.
	// It's implemented here just for convenience!
	Proof & Import (const Proof &);
	Proof & Qed ();
	Proof & Print ();
	Proof & PrintAll ();
	Proof & beginStep (string);
	Proof & endStep ();
	// TODO: the same problem as "Import"
	/* Proof SubAll (WffSubstitution) const ; */
	const Wff & getAxiom (size_t) const;
	const Axioms & getAxioms () const;
	const Wff & getContentWff (size_t) const;
	PCIndex getIndex (size_t);
	PCIndex lastIndex ();
	const Wff & target () const;
	const std::vector<ProofContent> & getContents () const;
	bool isDone () const;
};

PCIndex MP (PCIndex, PCIndex);

#endif
