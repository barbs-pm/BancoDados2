
#__________________~<>~__________________#
#                                        #
#  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL #
#  ALUNA: BÁRBARA PEGORARO MARKUS        #
#  PROFESSOR: GUILHERME DAL BIANCO       #
#  DISCIPLINA: BANCO DE DADOS II         #
#                                        #
#      IMPLELENTAÇÃO LOG REDO            #
#                                        #
#__________________~<>~__________________#

import re

#________________~<>~________________
#      DECLARAÇÃO DE VARIÁVEIS

arquivo = open("teste_final","r")
arquivolist = list(arquivo)    
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

for i in range(0,len(valores),2): #Iniciar primeiros valores das variáveis (A B C...)
    variaveis[valores[i]]= valores[i+1]

print("\n",variaveis)

#________________~<>~________________
# DESCOBRIR QUAL TRANSAÇÃO FARA REDO

end = 0

for linha in reversed(arquivolist): 
    if startckpt.search(linha):
        if end: 
            Tx = extracT.findall(linha)
            print("Start Checkpoint Txc para", Tx)         
            break

    elif commit.search(linha) and end < 2: # se tiver mais que um ckpt considerar só o ultimo
        Tx = extracT.findall(linha)[0]
        REDO.append(Tx)

    elif endckpt.search(linha) and end == 0:
        #print("End Checkpoint")
        end = end + 1 #se for mais que um, significa que tem mais de um ckpt

print("\nAplicar REDO nas transações:", REDO)
print("Processando...\n")

#________________~<>~________________
#    TROCA DO VALOR DAS VARIÁVEIS
#    CORRESPONDENTES AO REDO

for i in range(len(arquivolist)-1, 0, -1):

    linha = arquivolist[i]

    if start.search(linha) and not startckpt.search(linha):
        Tx = extracT.findall(linha)[0]
        if Tx in REDO:
            REDO.remove(Tx)      # remove a transação da lista do redo 
            REDOcheck.append(Tx) # adiciona a transação feita ao redocheck para saber que foi feita   
    
    elif words.findall(linha)[0] in REDO: #se a transação necessita redo 
        for j in range(0,len(valores),2): 
            if valores[j] == words.findall(linha)[2]:
                print("UPDATE", valores[j], ":", valores[j+1], " -> ", words.findall(linha)[3])
        
        variaveis[words.findall(linha)[2]] = words.findall(linha)[3] # altera os valores da posição 1 do log pelo valor da posição 3 (valor atualizado)

print("\nProcesso finalizado")
print("Aplicado REDO com sucesso em:", REDOcheck)
print("\nResultado:", variaveis)
arquivo.close()
