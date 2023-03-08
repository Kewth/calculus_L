#ifndef THEOREM_H
#define THEOREM_H
#include <string>

class Wff;
class WffSubstitution;
class Theorem {
	const Wff *m_pW;
	int m_axiomsId;
	Theorem (const Wff *, int);
public:
	Theorem ();
	Theorem sub (WffSubstitution) const;
	Theorem print () const;
	Theorem check (const Wff &) const;
	std::string Str () const;
	const Wff & W () const;
	friend Theorem MP (Theorem, Theorem);
	friend class Axioms;
};

Theorem MP (Theorem, Theorem);
#endif
