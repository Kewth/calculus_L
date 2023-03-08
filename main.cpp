#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "general.h"
#include "axioms.h"
#include "proof.h"
#include "wff.h"


const Wff A("A"), B("B"), C("C");
const Wff a("a"), b("b"), c("c");
const Axioms L = Axioms()
	.add(A >> (B >> A))
	.add((A >> (B >> C)) >> ((A >> B) >> (A >> C)))
	.add(((~A) >> (~B)) >> (((~A) >> B) >> A));

const Wff & Axiom1 = L.getAxiom(0), Axiom2 = L.getAxiom(1), Axiom3 = L.getAxiom(2);

PCIndex I1, I2, I3, I4, I5, I6;
PCIndex i1, i2, i3, i4, i5, i6;
Proof AtoA(L, A >> A);
Proof nBtoB_to_B(L, (~B >> B) >> B);
Proof hs(L.add(a >> b).add(b >> c), a >> c);
Proof AtoBC_to_BtoAC(L, ((A >> (B >> C)) >> (B >> (A >> C))));
Proof nAtonB_to_BtoA(L, ((~A) >> (~B)) >> (B >> A));
Proof any(L, A); // can not be proved

void proof1 () {
	Proof &P = AtoA;
	I1 = P.getIndex(0)                     .checkW  (A >> (B >> A));
	I2 = P.getIndex(1)                     .checkW  (A >> (B >> C) >> ((A >> B) >> (A >> C)));
	i1 = I2.sub((B = (A >> A)) && (C = A)) .checkW  (A >> ((A >> A) >> A) >> ((A >> (A >> A)) >> (A >> A)));
	i2 = I1.sub(B = (A >> A))              .checkW  (A >> ((A >> A) >> A));
	i3 = MP(i2, i1)                        .checkW  ((A >> (A >> A)) >> (A >> A));
	i4 = I1.sub(B = A)                     .checkW  (A >> (A >> A));
	i5 = MP(i4, i3)                        .checkW  (A >> A);
	P.Qed();
}

void proof2 () {
	Proof &P = nBtoB_to_B;
	I1 = P.getIndex(0);
	I2 = P.getIndex(1);
	I3 = P.getIndex(2);
	I4 = P.getIndex(3);
	I5 = P.getIndex(4);
	i1 = P.Import(AtoA).lastIndex().sub(A = ~B);
	i2 = I3.sub(A = B);
	i3 = MP(i1, i2);
	P.Qed();
}

PCIndex AH (Proof &P, const Wff &H, PCIndex T) { // output H >> T
	P.beginStep("AddHp");
	P.getAxiom(0).check(Axiom1);
	PCIndex res = MP(T, P.getIndex(0).sub((A = T.W()) && (B = H)));
	P.endStep();
	return res;
};

void proof3 () {
	Proof &P = hs;
	I1 = P.getIndex(0);
	I2 = P.getIndex(1).sub((A = a) && (B = b) && (C = c));
	auto iab = P.getIndex(3);
	auto ibc = P.getIndex(4);
	auto ia_bc = AH(P, a, ibc);
	auto iab_ac = MP(ia_bc, I2);
	MP(iab, iab_ac);
	P.Qed();
}

Proof Deduction (const Proof &P, size_t axiom_pos) {
	Axioms N = P.getAxioms().pop(axiom_pos);
	const Wff & H = P.getAxiom(axiom_pos);
	if (!H.is_fixed()) Error("Deduction on var");
	auto contents = P.getContents();
	std::vector<size_t> index_map; // index in P -> index in Q
	Proof Q(N, H >> P.target());
	PCIndex a2a = Q.Import(AtoA).lastIndex();
	for (ProofContent pc : contents) {
		switch (pc.getType()) {
			case Content_AXIOM:
				if (pc.W().same_as(H))
					index_map.push_back(a2a.sub(A = H).getPos());
				else {
					for (size_t i = 0; i < N.size(); i ++)
						if (Q.getContentWff(i).same_as(pc.W())) {
							index_map.push_back(AH(Q, H, Q.getIndex(i)).getPos());
							break;
						}
				}
				break;
			case Content_SUB:
				index_map.push_back(
						Q.sub(index_map[pc.getP1()], pc.getSub())
						.lastIndex().getPos());
				break;
			case Content_MP:
				const Wff & x = P.getContentWff(pc.getP1()); // x
				const Wff & x2y = P.getContentWff(pc.getP2()); // x >> y
				const Wff & y = wffMP(x, x2y);
				PCIndex mpaxiom = Q.getIndex(1).sub((A = H) && (B = x) && (C = y));
				/* mpaxiom.checkW((H >> (x >> y)) >> ((H >> x) >> (H >> y))); */
				PCIndex temp = Q.MP(index_map[pc.getP2()], mpaxiom.getPos()).lastIndex();
				temp.checkW((H >> x) >> (H >> y));
				index_map.push_back(
						Q.MP(index_map[pc.getP1()], temp.getPos())
						.lastIndex().getPos());
				break;
		}
	}
	return Q;
}

void proof4 () {
	Proof &P = AtoBC_to_BtoAC;
	P.Print();
}

int main () {
	proof1();
	proof2();
	proof3();
	Deduction(hs, 4).Print();
	/* proof4(); */
}
