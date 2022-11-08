#DPDA
# tranzitia este: x y l s p
# x - starea din care pleaca
# y - starea in care ajunge
# l - litera
# s - ce da pop pe stiva
# p - ce da push pe stiva
# pentru input, vom folosii $ in loc de Z0 si @ in loc de lambda


f = open("input.txt")
N = int(f.readline()) #nr de stari
stari = [int(x) for x in f.readline().split()]
M = int(f.readline()) #nr tranzitii
tranzitii = [[] for i in range(N + 1)]
for i in range(M):
    linie = f.readline().split()
    tranzitii[stari.index(int(linie[0])) + 1].append((int(linie[1]), linie[2],linie[3],linie[4]))
S = int(f.readline()) #stare initiala
nrF = int(f.readline()) #nr stari finale
finale = [int(x) for x in f.readline().split()]
NrCuv = int(f.readline())
cuvinte = []
for cuv in f:
    cuvinte.append(cuv.strip())
f.close()

# print(N)
# print(stari)
# print(M)
# print(tranzitii)
# print(S)
# print(nrF)
# print(finale)
# print(NrCuv)
# print(cuvinte)


g = open("output.txt", "w")
for cuvant in cuvinte:
    nodcr=S
    ok="DA"
    stack = ["$"]
    while cuvant!="":
        for tup in tranzitii[nodcr]:
            if tup[1]==cuvant[0] and (tup[2]==stack[len(stack)-1] or tup[2]=="@"):
                nodcr=tup[0]
                cuvant=cuvant[1:]
                if tup[2]!="@":
                    stack.pop()
                if tup[3]!="@":
                    for simbol in tup[3][::-1]:
                        stack.append(simbol)
                break
            elif tup[1]=="@" and (tup[2]==stack[len(stack)-1] or tup[2]=="@"):
                nodcr = tup[0]
                if tup[2]!="@":
                    stack.pop()
                if tup[3] != "@":
                    for simbol in tup[3][::-1]:
                        stack.append(simbol)
                break
        else:
            ok="NU"
            break
    for tup in tranzitii[nodcr]:
        if tup[1]=="@" and (tup[2]==stack[len(stack)-1] or tup[2]=="@"):
            nodcr = tup[0]
            if tup[2] != "@":
                stack.pop()
            if tup[3] != "@":
                for simbol in tup[3][::-1]:
                    stack.append(simbol)
            break
    if nodcr not in finale and len(stack)!=0:
        ok="NU"
    g.write(ok + "\n")
g.close()
