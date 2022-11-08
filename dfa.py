# a)DFA
f = open("input.txt")
N = int(f.readline())
stari = [int(x) for x in f.readline().split()]
M = int(f.readline())
tranzitii = [[] for i in range(N + 1)]
for i in range(M):
    linie = f.readline().split()
    tranzitii[stari.index(int(linie[0])) + 1].append((int(linie[1]), linie[2]))
S = int(f.readline())
nrF = int(f.readline())
finale = [int(x) for x in f.readline().split()]
NrCuv = int(f.readline())
cuvinte = []
for cuv in f:
    cuvinte.append(cuv.strip())
f.close()
g = open("output.txt", "w")
for cuvant in cuvinte:
    nodcr = S
    ok = "DA"
    while cuvant != "":
        for tup in tranzitii[nodcr]:
            if tup[1] == cuvant[0]:
                nodcr = tup[0]
                cuvant = cuvant[1:]
                break
        else:
            ok = "NU"
            break
    if nodcr not in finale:
        ok = "NU"
    g.write(ok + "\n")
g.close()
