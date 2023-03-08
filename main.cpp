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

Proof AtoA(L, A >> A);
Proof nBtoB_to_B(L, (~B >> B) >> B);
Proof hs(L.add(a >> b).add(b >> c), a >> c);
Proof AtoBC_to_BtoAC(L, ((A >> (B >> C)) >> (B >> (A >> C))));
Proof nAtonB_to_BtoA(L, ((~A) >> (~B)) >> (B >> A));
Proof nnAtoA(L, (~(~A)) >> A);
Proof contradictionToAny(L.add(a).add(~a), A);
Proof AtonnA(L, A >> (~(~A)));
Proof any(L, A); // can not be proved

void proof1 () {
	Proof &P = AtoA;
	auto I1 = P.getIndex(0)                     .checkW  (A >> (B >> A));
	auto I2 = P.getIndex(1)                     .checkW  (A >> (B >> C) >> ((A >> B) >> (A >> C)));
	auto i1 = I2.sub((B = (A >> A)) && (C = A)) .checkW  (A >> ((A >> A) >> A) >> ((A >> (A >> A)) >> (A >> A)));
	auto i2 = I1.sub(B = (A >> A))              .checkW  (A >> ((A >> A) >> A));
	auto i3 = MP(i2, i1)                        .checkW  ((A >> (A >> A)) >> (A >> A));
	auto i4 = I1.sub(B = A)                     .checkW  (A >> (A >> A));
	auto i5 = MP(i4, i3)                        .checkW  (A >> A);
	P.Qed();
}

void proof2 () {
	Proof &P = nBtoB_to_B;
	auto I1 = P.getIndex(0);
	auto I2 = P.getIndex(1);
	auto I3 = P.getIndex(2);
	auto I4 = P.getIndex(3);
	auto I5 = P.getIndex(4);
	auto i1 = P.Import(AtoA).lastIndex().sub(A = ~B);
	auto i2 = I3.sub(A = B);
	auto i3 = MP(i1, i2);
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
	auto I1 = P.getIndex(0);
	auto I2 = P.getIndex(1).sub((A = a) && (B = b) && (C = c));
	auto iab = P.getIndex(3);
	auto ibc = P.getIndex(4);
	auto ia_bc = AH(P, a, ibc);
	auto iab_ac = MP(ia_bc, I2);
	MP(iab, iab_ac);
	P.Qed();
}

Proof Deduction (const Proof &P, size_t axiom_pos) {
	if (!P.isDone())
		Error("Deduction not done");
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
			case Content_SUBFIX:
				index_map.push_back(
						Q.subFix(index_map[pc.getP1()], pc.getSub())
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
	return Q.Qed();
}

void proof4 () {
	Proof Q(L.add(a >> (b >> c)).add(b), a >> c);
	auto axiom2 = Q.getIndex(1).sub((A = a) && (B = b) && (C = c));
	auto hp1 = Q.getIndex(3);
	auto ib = Q.getIndex(4);
	auto iab_ac = MP(hp1, axiom2);
	auto iab = AH(Q, a, ib);
	MP(iab, iab_ac);
	Q.Qed();
	Proof Q1 = Deduction(Q, 4);
	Proof Q2 = Deduction(Q1, 3);
	Proof &P = AtoBC_to_BtoAC.Import(Q2);
	P.lastIndex().subFix((a = A) && (b = B) && (c = C));
	P.Qed();
}

void proof5 () {
	Proof Q(L.add((~a) >> (~b)).add(b), a);
	auto ax3 = Q.getIndex(2).sub((A=a)&&(B=b))      .checkW  (((~a)>>(~b))>>(((~a)>>b)>>a));
	auto inanb = Q.getIndex(3)                      .checkW  ((~a)>>(~b));
	auto ib = Q.getIndex(4)                         .checkW  (b);
	auto inab2a = MP(inanb, ax3)                    .checkW  (((~a)>>b)>>a);
	auto inab = AH(Q, ~a, ib)                       .checkW  ((~a)>>b);
	MP(inab, inab2a)                                .checkW  (a);
	Q.Qed();
	Proof &P = nAtonB_to_BtoA;
	P.Import(Deduction(Deduction(Q, 4), 3)).lastIndex().subFix((a=A)&&(b=B)&&(c=C));
	P.Qed();
}

void proof6 () {
	Proof Q(L.add(~(~a)), a);
	auto ax3 = Q.Import(nAtonB_to_BtoA).lastIndex() .checkW  (((~A)>>(~B))>>(B>>A));
	auto thm = Q.Import(nBtoB_to_B).lastIndex()     .checkW  (((~B)>>B)>>B);
	auto i1 = ax3.sub(B=~a)                         .checkW  (((~A)>>(~~a))>>((~a)>>A));
	auto inna = Q.getIndex(3)                       .checkW  (~~a);
	auto i4 = ax3.sub((A=a)&&(B=~a))                .checkW  (((~a)>>(~~a))>>((~a)>>a));
	auto i5 = AH(Q, ~a, inna)                       .checkW  ((~a)>>(~~a));
	auto i6 = MP(i5, i4)                            .checkW  ((~a)>>a);
	auto i7 = thm.sub(B=a)                          .checkW  (((~a)>>a)>>a);
	auto i8 = MP(i6, i7)                            .checkW  (a);
	Q.Qed();
	Proof &P = nnAtoA;
	P.Import(Deduction(Q, 3)).lastIndex().subFix(a=A);
	P.Qed();
}

void proof7 () {
	Proof &P = contradictionToAny;
	auto axiom1 = P.getIndex(0)                     .checkW  (A>>(B>>A));
	auto axiom2 = P.getIndex(1)                     .checkW  ((A>>(B>>C))>>((A>>B)>>(A>>C)));
	auto axiom3 = P.getIndex(2)                     .checkW  ((~A>>~B)>>((~A>>B)>>A));
	auto ia = P.getIndex(3)                         .checkW  (a);
	auto ina = P.getIndex(4)                        .checkW  (~a);
	auto i1 = AH(P, ~A, ia)                         .checkW  (~A>>a);
	auto i2 = AH(P, ~A, ina)                        .checkW  (~A>>~a);
	auto i3 = MP(i2, axiom3.sub(B=a))               .checkW  ((~A>>a)>>A);
	auto i4 = MP(i1, i3)                            .checkW  (A);
	P.Qed();
}

void proof8 () {
	Proof &P = AtonnA;
	auto i1 = AtonnA.Import(nAtonB_to_BtoA).lastIndex()  .checkW  ((~A>>~B)>>(B>>A));
	auto i2 = AtonnA.Import(nnAtoA).lastIndex()          .checkW  (~~A>>A);
	auto i3 = i1.sub((A=~~A)&&(B=A))                     .checkW  ((~~~A>>~A)>>(A>>~~A));
	auto i4 = i2.sub(A=~A)                               .checkW  (~~~A>>~A);
	MP(i4, i3)                                           .checkW  (A>>~~A);
	P.Qed();
}

int main () {
	proof1();
	proof2();
	proof3();
	proof4();
	proof5();
	proof6();
	proof7();
	AtoA.Print();
}
