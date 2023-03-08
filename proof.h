#ifndef PROOF_H
#define PROOF_H
#include <vector>
#include "axioms.h"

enum ProofOperator { Qed, Pprint };

class Wff;
class Theorem;
class Proof {
	Axioms m_axioms;
	const Wff *m_pW; // target
	std::vector<Theorem> m_history;
	bool m_done;
public:
	Proof (Axioms, const Wff &);
	Proof (Axioms, const Wff &, Proof);
	Theorem getAxiom (std::size_t) const;
	Axioms getAxioms () const;
	std::vector<Theorem> getHistory () const;
	Proof & operator << (Theorem);
	Proof & operator << (ProofOperator);
	/* Proof moveHypothesis (std::size_t) const; */
	Theorem Thm () const;
};

#endif
