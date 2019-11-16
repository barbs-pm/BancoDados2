/*..........................................
.    Universidade Federal da Fronteira Sul .
.    Bárbara Pegoraro Markus               .
.    Banco de Dados inserido               .
...........................................*/ 

#include <iostream>
#include <windows.h>
#include <mysql.h>

using namespace std;

void executaTransacao(MYSQL* conn, int stateQuery){
    if (!stateQuery){
        mysql_commit(conn);
        cout << "\nRegistro inserido, executando commit :)";
    }
    else {
        mysql_rollback(conn);
        cout << "\nHouve um erro, executando rollback :(" << mysql_error(conn);;
    }
}

void insereComSucesso(MYSQL* conn){
    int stateQuery;

    cout << "\nInserindo dados com sucesso";

    stateQuery = mysql_query(conn, "INSERT INTO `cliente` (`id`, `nome`) VALUES ('1', 'Barbara')");
    stateQuery = mysql_query(conn, "INSERT INTO `cliente` (`id`, `nome`) VALUES ('2', 'Luiz')");

    executaTransacao(conn, stateQuery);
}

void insereComErro(MYSQL* conn){
    int stateQuery;

    cout << "\nInserindo dados com erro (mesma chave primária)";

    stateQuery = mysql_query(conn, "INSERT INTO `cliente` (`id`, `nome`) VALUES ('1', 'Barbara')");
    stateQuery = mysql_query(conn, "INSERT INTO `cliente` (`id`, `nome`) VALUES ('1', 'Luiz')");

    executaTransacao(conn, stateQuery);
}

int main()
{
    MYSQL* conn;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "localhost", "root", "", "db2", 0, NULL, 0);

    if (conn)
        cout << "\nConectado <3";
    else
        cout << "\nHouve um erro ao se conectar :( " << mysql_error(conn);

    mysql_autocommit(conn, 0);

    insereComErro(conn);
    insereComSucesso(conn);

    cout << "\n";

    return 0;
}
