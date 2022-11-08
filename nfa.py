# b)NFA
ok="NU"
def DFS(starecr,tranz,cuv,fin):
    global ok
    if cuv!="":
        for tup in tranz[starecr]:
            if tup[1]==cuv[0]:
                DFS(tup[0],tranz,cuv[1:],fin)
    else:
        if starecr in finale:
            ok="DA"
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
    ok="NU"
    DFS(S,tranzitii,cuvant,finale)
    g.write(ok + "\n")
g.close()
