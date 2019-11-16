
#__________________~<>~__________________#
#                                        #
#  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL #
#  ALUNOS: BÁRBARA PEGORARO MARKUS       #
#          MAIKON DOUGLAS                #
#          TATIANE BARBOSA               #
#  PROFESSOR: GUILHERME DAL BIANCO       #
#  DISCIPLINA: BANCO DE DADOS II         #
#                                        #
#      IMPLELENTAÇÃO LOG UNDO/REDO       #
#                                        #
#__________________~<>~__________________#

import re

#________________~<>~________________
#      DECLARAÇÃO DE VARIÁVEIS

arquivo = open("teste03","r")
arquivolist = list(arquivo)    
UNDO = []                       
UNDOcheck = []
REDO = []                       
REDOcheck = []
  
commit = re.compile(r'commit', re.IGNORECASE) 
start = re.compile(r'start', re.IGNORECASE)
startckpt = re.compile(r'start ckpt', re.IGNORECASE)
endckpt = re.compile(r'end ckpt', re.IGNORECASE)
extracT = re.compile(r'(?!commit\b)(?!CKPT\b)(?!Start\b)\b\w+', re.IGNORECASE) 
words = re.compile(r'\w+', re.IGNORECASE)  

#________________~<>~________________
#       RETORNAR OS VALORES DAS 
#          VARIÁVEIS DO LOG

valores = words.findall(arquivolist[0])
variaveis = {}
for i in range(0,len(valores),2): 
    variaveis[valores[i]]= valores[i+1]

print("", variaveis)

#________________~<>~________________
# DESCOBRIR QUAL TRANSAÇÃO FARA REDO
#         E QUAL FARÁ UNDO

end = 0

for linha in reversed(arquivolist): 
    if startckpt.search(linha):
        if end: 
            Tx = extracT.findall(linha)
            print("Start Checkpoint Txc", Tx)         
            break

    elif start.search(linha):   
        Tx = extracT.findall(linha)[0]
        if Tx not in REDO:
            UNDO.append(Tx) # fazer undo se não for redo e não for commitado

    elif commit.search(linha) and end < 2: # se tiver mais que um ckpt considerar só o ultimo
        Tx = extracT.findall(linha)[0]
        REDO.append(Tx)

    elif endckpt.search(linha) and end == 0:
        print(" End  Checkpoint")
        end = end + 1 #se for mais que um, significa que tem mais de um ckpt

print("\nAplicar UNDO:", UNDO)
print("Aplicar REDO:", REDO)

#________________~<>~________________
#    TROCA DO VALOR DAS VARIÁVEIS
#    CORRESPONDENTES AO UNDO/REDO

for i in range(len(arquivolist)-1, 0, -1): 
    linha = arquivolist[i]
    if start.search(linha) and not startckpt.search(linha):
        Tx = extracT.findall(linha)[0]
        if Tx in UNDO:
            UNDO.remove(Tx)      # remove a transação da lista do undo 
            UNDOcheck.append(Tx) # adiciona a transação feita ao undocheck para saber que foi feita
        if Tx in REDO:
            REDO.remove(Tx)      # remove a transação da lista do redo 
            REDOcheck.append(Tx) # adiciona a transação feita ao redocheck para saber que foi feita   
        if not len(UNDO):
            break
    
    elif words.findall(linha)[0] in UNDO:
        variaveis[words.findall(linha)[1]] = words.findall(linha)[2] # altera os valores da posição 1 do log pelo valor da posição 2 (valor antigo)
    
    elif words.findall(linha)[0] in REDO:
        variaveis[words.findall(linha)[1]] = words.findall(linha)[3] # altera os valores da posição 1 do log pelo valor da posição 3 (valor atualizado)

for j in range(i,len(arquivolist)-1,1):
    linha = arquivolist[j]
  

print("\nAplicado UNDO:", UNDOcheck)
print("Aplicado REDO:", REDOcheck)
print("\nResultado:", variaveis)
arquivo.close()

"""
OBSERVAÇÕES:

O erro estava no modo que era feito o undo. Todas as transações eram feitas com undo, mesmo as com commit.
Logo, não era validado se havia commit para fazer redo. Agora o undo verifica se a transação já faz parte do redo,
e as transações com commit do ultimo checkpoint fazem redo.

"""