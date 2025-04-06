#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignamos un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignamos un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Nos conectamos a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conectado a la base de datos exitosamente." << endl;

        // Ejecución de una consulta
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Imprimir encabezados de la tabla
        cout << "+----------+-----------------------------+-----------------+--------------+--------------+---------+---------+" << endl;
        cout << "|N.Empleado|Nombre                       | FechaNacimiento |  RFC         |Centro_Trabajo|ID_Puesto|Directivo|" << endl;
        cout << "+----------+-----------------------------+-----------------+--------------+--------------+---------+---------+" << endl;

        // Ejemplo de consulta SELECT
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int Num_Empleado;
            SQLCHAR Nombre[50];
            SQLCHAR Apellido_Materno[50];
            SQLCHAR Apellido_Paterno[50];
            SQLCHAR Fecha_Nacimiento[50];
            SQLCHAR RFC[50];
            SQLCHAR Centro_Trabajo[50];
            SQLCHAR ID_Puesto[50];
            SQLCHAR Directivo[50];
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &Num_Empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, Nombre, sizeof(Nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, Apellido_Paterno, sizeof(Apellido_Paterno), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, Apellido_Materno, sizeof(Apellido_Materno), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, Fecha_Nacimiento, sizeof(Fecha_Nacimiento), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, Centro_Trabajo, sizeof(Centro_Trabajo), NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, ID_Puesto, sizeof(ID_Puesto), NULL);               
                SQLGetData(hStmt, 10, SQL_C_CHAR, Directivo, sizeof(Directivo), NULL);
       
           
            //concatenamos nombre
            string Apellidos = string((char*)Apellido_Paterno) + " " + string((char*)Apellido_Materno);
            string NombreCompleto = string((char*)Nombre) + " " + string(Apellidos);
            string dir = string((char*)Directivo);
            string respuesta;

            if (dir == "1") {
                respuesta = "SI";
            }
            else
            {
                respuesta = "NO";
            }


            // Imprimir datos de la fila con alineación
            cout << "| " << setw(9) << left << Num_Empleado
                << "| " << setw(28) << left << NombreCompleto
				<< "| " << setw(16) << left << Fecha_Nacimiento
				<< "| " << setw(13) << left << RFC
				<< "| " << setw(13) << left << Centro_Trabajo
				<< "| " << setw(8) << left << ID_Puesto
				<< "| " << setw(1) << left << Directivo
                << "| " << setw(4) << left << respuesta << " |" << endl;
			}
           
            // Imprimir línea final de la tabla
            cout << "+----------+-----------------------------+-----------------+--------------+--------------+---------+---------+" << endl;

            // Liberar el manejador de conexión
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            cout << "Fallo la ejecución de la consulta" << endl;
        }
    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}