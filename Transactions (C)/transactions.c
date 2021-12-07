//gcc -o transactions transactions.c -I/usr/include/postgresql -lpq

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <time.h>

#define MAX_LINHA 1024
#define DELIMITADOR "\t"

struct timespec begin, end;

void do_exit(PGconn *conn) {
    printf("%s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
}

void insertExplicito(PGconn *conn, PGresult *res, FILE* fp) {

    system("clear");

    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    char linha[MAX_LINHA];
    int row = 0, i = 0;

    res = PQexec(conn, "BEGIN");

    printf("Limpando a tabela.\n");
    res = PQexec(conn, "delete from nile");

    printf("Iniciando inserção explícita.\n");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erro: %s", PQerrorMessage(conn));
        PQclear(res);
        do_exit(conn);
    }

    while (fgets(linha, MAX_LINHA, fp) != NULL) {
        char str[MAX_LINHA] = "INSERT INTO nile (year, height) VALUES (";
        row++; 

        // ignorar o cabeçalho
        if (row == 1) 
            continue; 

        char* value = strtok(linha, DELIMITADOR); 

        while (value) {
            strcat(str, value);
            strcat(str, ")");

            res = PQexec(conn, str);

            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                res = PQexec(conn, "ROLLBACK");
                do_exit(conn);
            }

            PQclear(res);
            value = strtok(NULL, ", ");

        }
    } 

    res = PQexec(conn, "COMMIT");
    
    PQclear(res);
    fclose(fp); 
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("___________________~<>~__________________\n");
    printf("Tempo de execução = %.2f segundos\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec  - begin.tv_sec));
    printf("Aperte 1 para voltar: ");
	scanf("%d", &i);

}

void insertImplicito(PGconn *conn, PGresult *res, FILE* fp) {

    system("clear");

    char linha[MAX_LINHA]; 
	int row = 0, i = 0;

	clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    printf("Limpando a tabela.\n");
    res = PQexec(conn, "delete from nile");

    printf("Iniciando inserção implícita.\n");

	while (fgets(linha, MAX_LINHA, fp)) {

		char str[MAX_LINHA] = "INSERT INTO nile (year, height) VALUES ("; 
		row++; 

        // ignorar cabeçalho
		if (row == 1) 
			continue; 

		char* value = strtok(linha, DELIMITADOR); 

		while (value) {
			strcat(str, value);
			strcat(str, ")");
            
            //printf("%s\n", str);

			res = PQexec(conn, str);

			if (PQresultStatus(res) != PGRES_COMMAND_OK) {
				res = PQexec(conn, "ROLLBACK");
				do_exit(conn);
			}
			PQclear(res);
			value = strtok(NULL, ", ");
		}
	} 

	fclose(fp); 
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	
    printf("___________________~<>~__________________\n");
    printf("Tempo de execução = %f segundos\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec  - begin.tv_sec));
    printf("Aperte 1 para voltar: ");
	scanf("%d", &i);
    
}

void rollback_erro(PGconn *conn, PGresult *res, FILE* fp) {

	char linha[MAX_LINHA]; 
	int row = 0, i = 0;
    
	clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
	res = PQexec(conn, "BEGIN");

	printf("Iniciando inserção.\n");

	while (fgets(linha, MAX_LINHA, fp)) {

		char str[MAX_LINHA] = "INSERT INTO nile (year, height) VALUES ("; 
		row++; 

		if (row == 1) 
			continue; 

		char* value = strtok(linha, ""); 

		while (value) {
            // forçar erro
			if (i == 100) {
				strcat(str, value);
				strcat(str, ")]");
			} else {
				strcat(str, value);
				strcat(str, ")");
			}

			res = PQexec(conn, str);

			if (PQresultStatus(res) != PGRES_COMMAND_OK) {
				printf("Erro. Executando rollback\n");
				res = PQexec(conn, "ROLLBACK");
				do_exit(conn);
			}
			PQclear(res);
			value = strtok(NULL, ", ");
		}
		i++;
	} 
	printf("END COMMIT FUNCTION ROLLBACK\n");
	res = PQexec(conn, "COMMIT");
	fclose(fp); 
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	printf("Tempo de execução = %f segundos\n", (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec  - begin.tv_sec));
    
}

int main() {
    PGconn          *conn;
    PGresult        *res;
    int             op;

    conn = PQconnectdb("dbname=tp1 host=localhost user=barbs password=123456");

    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("Nao eh possivel conectar ao banco");
        exit(0);
    } 

    printf("Conexão com o banco sucedida.\nAbrindo o arquivo csv.\n");

    FILE* fp = fopen("nile.csv", "r");

    if (!fp) {
        printf("Não foi possível abrir o arquivo csv.\n");
        do_exit(conn);
    } else {

        do{
            system("clear");
            printf("___________~<>~__________\n");
            printf("|1| Implícito\n");
            printf("|2| Explícito\n");
            printf("|3| Rollback\n");
            printf("___________~<>~__________\n");
            printf("Escolha sua opção: ");
            scanf("%d", &op);

            switch(op){
                case 1: insertImplicito(conn,res,fp); break;
                case 2: insertExplicito(conn,res,fp); break;
                case 3: rollback_erro(conn,res,fp); break;
            }

            //op = 0;
        
        } while(op != 0);
    }

    return 0;
}
