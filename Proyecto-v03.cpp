#include <iostream>
#include <string>
#include <windows.h> //Gotoxy, colores, sonidos
#include <iomanip>   //Para dar formato a los decimales
#include <vector>    //
#include <clocale>   //Permite indentificar tildes y eñes
#include <conio.h>   //get
// #include <thread> Para congelar el programa por un tiempo estimado
// #include <chrono> Para definir la duracion del tiempo a esperar

using namespace std;

// VARIABLES
double tasaCambio = 370.25, acumTotal = 0, acumImpuestos = 0;
int conVentas = 0, venEfectivo = 0, venTarjeta = 0, venTransferencia = 0;

// Estructura para agrupar los datos de cada producto
struct producto
{
    string nombre;
    double precioDolar;
    int stock, vendidos;
};

// PRODUCTOS BASE
vector<producto> inventario = {
    {"Arroz", 2.5, 10, 0},
    {"Azúcar", 3.0, 8, 0},
    {"Café", 5.0, 6, 0},
    {"Leche", 4.0, 12, 0},
    {"Harina", 2.0, 15, 0}};

// PROTOTIPOS DE LAS FUNCIONES//

// Esteticos
void dibujarMarco(int x1, int y1, int x2, int y2);
void gotoxy(int x, int y);
void mostrarCursor();
void ocultarCursor();
void sonidoBienvenida();
void sonidoTecla();
void sonidoExito();
void sonidoError();
void sonidoCarga();
void cargaFinalizada();

// Principales
void login();
void barraCarga(int progresoTotal);
void cuentaRegresiva(int segundos);
void validacionDatos(vector<producto> &inventario, int &idSel, int &cantSel, int &idx);
void menuPrincipal(vector<producto> &inventario);
void registrarVenta(vector<producto> &inventario);
void admInventario(vector<producto> &inventario);
void reportes(const vector<producto> &inventario);
void resumenDia(const vector<producto> &inventario);
void cierreDia();
void actualizarTasaDolar();

// FUNCION PRINCIPAL
int main()
{
    // Permite el uso de tildes y eñes
    SetConsoleOutputCP(CP_UTF8);

    system("title Sistema de Facturacion - VALROS S.A.");
    system("color 0F");

    login();

    barraCarga(100);

    menuPrincipal(inventario);

    return 0;
}

// FUNCIONES//

// MENU
void menuPrincipal(vector<producto> &inventario)
{
    sonidoBienvenida();
    // Definimos las 7 opciones q tendra el menucito
    string opciones[] = {
        "1) Registrar venta",
        "2) Inventario (editar precios/stock)",
        "3) Reportes",
        "4) Resumen del día",
        "5) Cierre del día",
        "6) Actualizar Tasa del Dólar",
        "0) Salir"};
    int totalOpciones = 7, seleccionada = 0;
    bool ejecutando = true;

    dibujarMarco(12, 2, 68, 24); // Marco grande
    dibujarMarco(18, 3, 62, 5);  // Marco pequeño
    gotoxy(21, 4);
    cout << "  SISTEMA DE FACTURACIÓN - VALROS S.A.";
    // Le indica al usuario q teclas usar
    gotoxy(15, 23);
    cout << "  Use las flechas (↑ / ↓) y presione [ENTER]";

    // Esto se va a repetir mientras ejecutando sea verdadero(el usuario no indique la opcion de salir el cual la convierte falsa)
    while (ejecutando)
    {

        // Dibujamos otra vez las opciones en sus posiciones actuales
        for (int i = 0; i < totalOpciones; i++)
        {
            if (i == seleccionada)
            {
                // Opcion seleccionada (fila inicial 8 + i * 2)
                gotoxy(18, 8 + (i * 2));

                cout << "► [ " << opciones[i] << " ] ◄";
            }
            else
            {
                // Opcion normal (los espacios tapan las flechas anteriores)
                gotoxy(18, 8 + (i * 2));
                cout << "    " << opciones[i] << "    ";
            }
        }

        // Para capturar la tecla q ingrese el usuario
        char tecla = _getch();

        if (tecla == 72)
        { // Flecha arriba (↑)
            sonidoTecla();
            seleccionada--;
            if (seleccionada < 0)
                seleccionada = totalOpciones - 1;
        }
        else if (tecla == 80)
        { // Flecha abajo (↓)
            sonidoTecla();
            seleccionada++;
            if (seleccionada >= totalOpciones)
                seleccionada = 0;
        }
        else if (tecla == 13)
        { // Tecla ENTER
            sonidoTecla();
            system("cls");
            // Al presionar Enter, ejecutamos la opcion elegida
            switch (seleccionada)
            {
            case 0:
                registrarVenta(inventario);
                break;
            case 1:
                admInventario(inventario);
                break;
            case 2:
                reportes(inventario);
                break;
            case 3:
                resumenDia(inventario);
                break;
            case 4:
                cierreDia();
                break;
            case 5:
                actualizarTasaDolar();
                break;
            case 6: // Salir
                dibujarMarco(18, 2, 60, 6);
                gotoxy(21, 4);
                cout << "¡Gracias por usar el sistema, adiós!";
                _getch();
                Sleep(1500);
                ejecutando = false;
                break;
            }

            if (ejecutando)
            {
                // Esto borra lo q haya dejado las funciones para cuando el usuario vuelva al menu
                system("cls");
                dibujarMarco(12, 2, 68, 24); // Marco grande
                cout.flush();
                dibujarMarco(18, 3, 62, 5); // Marco pequeño
                gotoxy(21, 4);
                cout << "  SISTEMA DE FACTURACIÓN - VALROS S.A.";
                // Le indica al usuario q teclas usar
                gotoxy(15, 23);
                cout << "  Use las flechas (↑ / ↓) y presione [ENTER]";
            }
        }
    }
}

// FUNCIONES ESTETICAS
// Recubica el cursor de texto en coordenadas exactas.
void gotoxy(int x, int y)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}

// Elimina el cuadrito parpadeante de la consola.
void ocultarCursor()
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 100;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hcon, &cci);
}

// Reactiva el cursor parpadeante en la consola.
void mostrarCursor()
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 100;
    cci.bVisible = TRUE;
    SetConsoleCursorInfo(hcon, &cci);
}

// MARCO
void dibujarMarco(int x1, int y1, int x2, int y2)
{
    // Para dibujar las esquinas
    gotoxy(x1, y1);
    cout << "╔";
    gotoxy(x2, y1);
    cout << "╗";
    gotoxy(x1, y2);
    cout << "╚";
    gotoxy(x2, y2);
    cout << "╝";

    // Dibuja las líneas horizontales (techo y suelo)
    for (int i = x1 + 1; i < x2; i++)
    {
        gotoxy(i, y1);
        cout << "═"; // Línea de arriba
        gotoxy(i, y2);
        cout << "═"; // Línea de abajo
    }

    // Dibuja las líneas verticales (paredes)
    for (int j = y1 + 1; j < y2; j++)
    {
        gotoxy(x1, j);
        cout << "║"; // Pared izquierda
        gotoxy(x2, j);
        cout << "║"; // Pared derecha
    }
}

// FUNCIONES DE SONIDO
// Beep(frecuencia, duración);

// Sonido de bienvenida
void sonidoBienvenida()
{
    Beep(523, 100);
    Beep(659, 100);
    Beep(784, 100);
    Beep(1046, 200);
}

// Sonido basico de tecla
void sonidoTecla()
{
    Beep(800, 60);
}

// Sonido de exito
void sonidoExito()
{
    Beep(1000, 100);
    Sleep(20); // Pequena pausa
    Beep(1300, 150);
}

// Sonido de error
void sonidoError()
{
    Beep(350, 350);
}

// Sonido de carga
void sonidoCarga()
{
    Beep(523, 100);
    Beep(659, 100);
    Beep(784, 100);
    Beep(1047, 100);
    Beep(1175, 100);
    Beep(1318, 100);
}

// Sonido de la carga finalizada
void cargaFinalizada()
{
    Beep(1397, 100);
}

// LOGIN
// Funcion encargada de aplicar la espera de tiempo cuando se bloquee el login
void cuentaRegresiva(int segundos)
{
    gotoxy(23, 12);
    cout << "Demasiados intentos fallidos. Sistema bloqueado";

    // Bucle q maneja los segundos restantes
    for (int i = segundos; i > 0; i--)
    {
        //'\r' Manda al curso al inicio de la misma linea haciendo q se vea como si los numeros retrocedieran con cada ciclo
        gotoxy(23, 15);
        cout << "Por favor, intente de nuevo en: " << i << " segundos...";
        // Vacia el bufer para forzar a la pantalla a mostrar el numero ya
        cout.flush();
        // Detiene la ejecucion por 1 segundo en cada ciclo
        Sleep(1000);
    }
    system("cls");
    // gotoxy perfecto
    gotoxy(23, 12);
    cout << "Sistema desbloqueado. Puede intentarlo de nuevo";
    Sleep(2000);
    system("cls");
}

void login()
{
    // DATOS LOGIN
    const string usuario = "valeria", clave = "4321";
    int intentosMax = 3, intentosRealizados = 0, intentosRestantes = 0;
    bool accesoConcedido = false;
    string usu, cla;

    while (!accesoConcedido)
    {
        // Marco para el login
        sonidoTecla();
        dibujarMarco(25, 8, 55, 19);
        gotoxy(32, 10);
        cout << "INICIO DE SESION";
        gotoxy(26, 11);
        cout << "-----------------------------";
        gotoxy(28, 13);
        cout << "Usuario: ";
        mostrarCursor();
        cin >> usu;
        sonidoTecla();
        gotoxy(28, 14);
        cout << "Contraseña: ";
        cin >> cla;
        ocultarCursor();
        sonidoTecla();

        // Condiciones del login para la validación de datos
        if (usu == usuario && cla == clave)
        {
            gotoxy(28, 16);
            cout << "Acceso concedido";
            sonidoExito();
            Sleep(2000);
            system("cls");
            accesoConcedido = true;
        }

        else
        {
            sonidoError();
            system("cls");
            intentosRealizados++;
            intentosRestantes = intentosMax - intentosRealizados;

            if (intentosRestantes > 0)
            {
                gotoxy(23, 15);
                cout << "Datos incorrectos. Le quedan: " << intentosRestantes << " intentos";
                Sleep(2000);
                system("cls");
            }
            else
            {
                cuentaRegresiva(15);
                intentosRealizados = 0;
            }
        }
    }
}

// BARRA DE CARGA
void barraCarga(int progresoTotal)
{
    int anchoBarra = 40;

    for (int i = 0; i <= progresoTotal; i++)
    {
        gotoxy(39, 12);
        cout << "[";

        int posicion = anchoBarra * i / progresoTotal;

        for (int j = 0; j < anchoBarra; j++)
        {
            if (j < posicion)
            {
                cout << "█";
            }
            else if (j == posicion)
                cout << " ";

            else
                cout << "▒";
        }
        //
        gotoxy(80, 12);
        cout << "] " << (i * 100 / progresoTotal) << " %\r";
        cout.flush();

        // sonidoCarga();

        // Una pequena pausa para q el ciclo se detenga y parezca q carga la barra
        Sleep(2);
    }
    gotoxy(45, 14);
    cargaFinalizada();
    cout << "Carga completada con exito";
    Sleep(1000);
    system("cls");
}

// FUNCIONES DE LAS OPCIONES DEL MENU//

void validacionDatos(vector<producto> &inventario, int &idSel, int &cantSel, int &idx)
{
    // Mostrar información de productos
    cout << "ID\tPRODUCTO\tPRECIO (Bs.)\tSTOCK" << endl;

    for (size_t i = 0; i < inventario.size(); i++)
    {
        cout << i + 1 << "\t"
             << inventario[i].nombre << "\t\t"
             << fixed << setprecision(2) << (inventario[i].precioDolar * tasaCambio) << " Bs.\t"
             << inventario[i].stock << endl;
    }

    do
    {
        cout << "Seleccione ID del producto: ";
        cin >> idSel;

        idx = idSel - 1;

        // Validacion de ID
        if (idx < 0 || static_cast<size_t>(idx) >= inventario.size())
        {
            cout << "Error: ID de producto no existe." << endl;
        }
    } while (idx < 0 || static_cast<size_t>(idx) >= inventario.size());

    do
    {
        cout << "Ingrese la cantidad que desea llevar: ";
        cin >> cantSel;

        // Validación de la  cantidad de productos
        if (cantSel <= 0 || cantSel > inventario[idx].stock)
        {
            cout << "Error: Stock insuficiente o cantidad inválida." << endl
                 << "Ingrese una cantidad valida (Maximo " << inventario[idx].stock << "): ";
        }

    } while (cantSel <= 0 || cantSel > inventario[idx].stock);

    inventario[idx].stock -= cantSel;
    inventario[idx].vendidos += cantSel;
}

// Funcion para registrar una venta y crear factura
void registrarVenta(vector<producto> &inventario)
{
    // Variables (indentidades del usuario)
    string nombreCliente, apellidoCliente;
    int documento;
    char nacionalidad;
    nacionalidad=0;
system ("cls");
    dibujarMarco(10, 2, 55, 6);
    gotoxy(12,4);
    cout << "          MÓDULO DE FACTURACIÓN          " << endl;

    
    
    while (nombreCliente.empty())
    {
      gotoxy(10,8);
        cout << "Ingrese nombre del cliente: ";
       getline (cin,nombreCliente);
    }
    
    while (apellidoCliente.empty())
    {  gotoxy(10,9);
        cout << "Ingrese apellido del cliente: ";
       getline (cin,apellidoCliente);
    }
    
 system ("cls");
    
    do
    {
        gotoxy(10, 2);
        cout << "Indique la nacionalidad:\n1 - Venezolano\n2 - Extranjero\nOpción: ";
        cin >> nacionalidad;
        
        // Validacion de digito correcto
        
        if (nacionalidad != '1' && nacionalidad != '2')
        {
            gotoxy(15, 9);
            cout << "Dígito incorrecto. Intente de nuevo.\n";
            Sleep(300);
            system("cls");

        }
    } while (nacionalidad != '1' && nacionalidad != '2');
 system ("cls");
    gotoxy(10,2);
    cout << "Indique número de documento (Cédula/Pasaporte): ";
    cin >> documento;

    // Una vez ingresados los datos del usuario se limpia la pantalla para la seleccion de productos
    system("cls");

    // Selección de producto
    // marco
    int seguir;
    int idSel = 0, cantSel = 0, idx = -1;
    double subtotal = 0;
    double precioBs = 0;
    double subtotalGeneral = 0;

    // para q en la factura salga cada uno de los productos comprados y no solo el ultimo
    struct carrito
    {
        string nombreProd;
        int cantProd;
        double precioUni, subtotalItem;
    };

    vector<carrito> elementos;

    do
    {
        validacionDatos(inventario, idSel, cantSel, idx);
        precioBs = inventario[idx].precioDolar * tasaCambio;
        subtotal = cantSel * precioBs;

        // subtotal general
        subtotalGeneral += subtotal;

        // Guardamos el producto en un carrito para luego mostrar su información individual en factura
        carrito item;
        item.nombreProd = inventario[idx].nombre;
        item.cantProd = cantSel;
        item.precioUni = precioBs;
        item.subtotalItem = subtotal;

        // Agregamos el producto al vector
        elementos.push_back(item);

        cout << "Desea comprar otro producto";
        cout << "1) Si\n";
        cout << "2) No\n";
        cin >> seguir;
    } while (seguir == 1);

    // Variables para el recibo
    int medioPago;
    double recargoTarjeta = 0;

    do
    {
        cout << "\nMedio de pago:\n1) Efectivo\n2) Tarjeta (2% recargo)\n3) Transferencia\nOpción: ";
        cin >> medioPago;

        // Validación del medio de pago
        if (medioPago < 1 || medioPago > 3)
        {
            cout << "Medio inválido. Por favor, seleccione 1, 2 o 3. \n." << endl;
            cin.clear();           // Limpia el error del cin
            cin.ignore(100, '\n'); // Descarta el texto incorrecto q quedo en el bufer
        }

    } while (medioPago < 1 || medioPago > 3);

    // IVA
    double impuestos = subtotal * 0.16;

    // Para el recargo de la tarjeta si el usuario escoje ese medio de pago
    if (medioPago == 2)
    {
        recargoTarjeta = 0.02 * (subtotal + impuestos);
    }

    // Total a pagar
    double total = subtotal + impuestos + recargoTarjeta;

    // Acumuladores y contadores para guardar datos
    conVentas++;
    acumTotal += total;
    acumImpuestos += impuestos;

    if (medioPago == 1)
        venEfectivo++;
    else if (medioPago == 2)
        venTarjeta++;
    else
        venTransferencia++;

    // Imprimir recibo
    system("cls");
    // marco
    cout << "                RECIBO DE COMPRA               " << endl

         << "Cliente: " << nombreCliente << " " << apellidoCliente << endl
         << "Documento: " << (nacionalidad == 1 ? "V-" : "E-") << documento << endl
         << "CANTIDAD\tPRODUCTO\tPRECIO (Bs.)\tSUBTOTAL\n";
    for (size_t i = 0; i < elementos.size(); i++)
    {
        cout << elementos[i].cantProd << "\t\t" << elementos[i].nombreProd << "\t\t" << fixed << setprecision(2) << elementos[i].precioUni << " Bs.\t" << elementos[i].subtotalItem << " Bs.\n";
    }

    cout << "Subtotal Neto: " << subtotalGeneral << " Bs." << endl
         << "IVA (16%): " << impuestos << " Bs." << endl
         << "Recargo Tarjeta: " << recargoTarjeta << " Bs." << endl
         << "TOTAL A PAGAR: " << total << " Bs." << endl
         << "Medio de Pago: " << (medioPago == 1 ? "Efectivo" : (medioPago == 2 ? "Tarjeta" : "Transferencia")) << endl
         << "!GRACIAS POR SU COMPRA!\n";
    gotoxy(15, 30);
    system("pause");
}

void admInventario(vector<producto> &inventario)
{
    int opInv;
    do
    {
        // marco
        cout << "ID\tPRODUCTO\tPRECIO (Bs.)\tSTOCK\tVENDIDOS" << endl;

        for (size_t i = 0; i < inventario.size(); i++)
        {
            cout << i + 1 << "\t"
                 << inventario[i].nombre << "\t\t"
                 << fixed << setprecision(2) << (inventario[i].precioDolar * tasaCambio) << " Bs.\t"
                 << inventario[i].stock << "\t"
                 << inventario[i].vendidos << endl;
        }
        // marco
        cout << "1) Cambiar precio de un producto" << endl
             << "2) Ajustar stock de un producto" << endl
             << "3) Cambiar nombre de un producto" << endl
             << "4) Agregar nuevo producto" << endl
             << "0) Volver al menú principal" << endl
             << "Seleccione una opción: ";
        cin >> opInv;

        // Cambiar el precio de un producto
        if (opInv == 1)
        {
            int prodSel;
            double nuevoPrecio;
            cout << "Ingrese el número del producto: ";
            cin >> prodSel;
            int idx = prodSel - 1;
            if (idx >= 0 && static_cast<size_t>(idx) < inventario.size())
            {
                cout << "Ingrese el nuevo precio en Bolívares (Tasa actual: " << tasaCambio << "): ";
                cin >> nuevoPrecio;
                if (nuevoPrecio >= 0)
                {
                    inventario[idx].precioDolar = nuevoPrecio / tasaCambio; // Se almacena convertido a $
                    cout << "Precio actualizado correctamente." << endl;
                }
                else
                {
                    cout << "Error: El precio no puede ser menor a 0." << endl;
                }
            }
            else
                cout << "Error: El producto no existe." << endl;
            Sleep(1500);
        }
        else if (opInv == 2)
        {
            int prodSel, nuevoStock;
            cout << "Ingrese el número del producto: ";
            cin >> prodSel;
            int idx = prodSel - 1;
            if (idx >= 0 && static_cast<size_t>(idx) < inventario.size())
            {
                cout << "Ingrese la nueva cantidad de stock: ";
                cin >> nuevoStock;
                if (nuevoStock >= 0)
                {
                    inventario[idx].stock = nuevoStock;
                    cout << "Stock actualizado exitosamente." << endl;
                }
                else
                {
                    cout << "Error: La cantidad no puede ser negativa." << endl;
                }
            }
            else
                cout << "Error: El producto no existe." << endl;
            Sleep(1500);
        }
        else if (opInv == 3)
        {
            int prodSel;
            string nuevoNombre;
            cout << "Ingrese el número del producto: ";
            cin >> prodSel;
            int idx = prodSel - 1;
            if (idx >= 0 && static_cast<size_t>(idx) < inventario.size())
            {
                cout << "Ingrese el nuevo nombre para " << inventario[idx].nombre << ": ";
                cin >> nuevoNombre;
                inventario[idx].nombre = nuevoNombre;
                cout << "Nombre actualizado exitosamente." << endl;
            }
            else
                cout << "Error: El producto no existe." << endl;
            Sleep(1500);
        }
        else if (opInv == 4)
        {
            producto nuevoProd;
            double precioBs;
            cout << "Nombre del producto: ";
            cin >> nuevoProd.nombre;
            cout << "Precio de venta en Bolívares (Bs): ";
            cin >> precioBs;
            cout << "Stock inicial: ";
            cin >> nuevoProd.stock;
            nuevoProd.precioDolar = precioBs / tasaCambio;
            nuevoProd.vendidos = 0;

            inventario.push_back(nuevoProd); // Añadir al inventario el nuevo producto
            cout << "Producto agregado correctamente." << endl;
            Sleep(1500);
        }

    } while (opInv != 0);
}

void reportes(const vector<producto> &inventario)
{
    int opRep;
    do
    {
        // marco
        cout << "               REPORTES                 " << endl

             << "1) Producto más vendido" << endl
             << "2) Alerta de Stock bajo" << endl
             << "3) Ventas por medio de pago" << endl
             << "0) Volver" << endl
             << "Seleccione una opción: ";
        cin >> opRep;

        system("cls");
        if (opRep == 1)
        {
            int maxVendidos = -1;
            string nombreMasVendido = "Ninguno";
            for (size_t i = 0; i < inventario.size(); i++)
            {
                if (inventario[i].vendidos > maxVendidos && inventario[i].vendidos > 0)
                {
                    maxVendidos = inventario[i].vendidos;
                    nombreMasVendido = inventario[i].nombre;
                }
            }
            cout << "=== PRODUCTO MÁS VENDIDO ===" << endl;
            if (maxVendidos == -1)
                cout << "No se han realizado ventas aún." << endl;
            else
                cout << nombreMasVendido << " con " << maxVendidos << " unidades vendidas." << endl;
            system("pause");
        }
        else if (opRep == 2)
        {
            cout << "=== PRODUCTOS CON STOCK BAJO (<= 5) ===" << endl;
            int contBajo = 0;
            for (size_t i = 0; i < inventario.size(); i++)
            {
                if (inventario[i].stock <= 5)
                {
                    cout << "- " << inventario[i].nombre << " | Unidades restantes: " << inventario[i].stock << endl;
                    contBajo++;
                }
            }
            // No hay ningun producto poquito
            if (contBajo == 0)
                cout << "Todos los productos poseen stock óptimo." << endl;
            system("pause");
        }
        else if (opRep == 3)
        {
            cout << "=== VENTAS POR MEDIO DE PAGO ===" << endl;
            cout << "Transacciones en Efectivo:      " << venEfectivo << endl;
            cout << "Transacciones en Tarjeta:       " << venTarjeta << endl;
            cout << "Transacciones en Transferencia: " << venTransferencia << endl;
            system("pause");
        }
    } while (opRep != 0);
}

void resumenDia(const vector<producto> &inventario)
{
    // marco
    cout << "               RESUMEN DEL DÍA                  " << endl
         << "Ventas totales realizadas: " << conVentas << endl
         << "Total ingresos acumulados: " << fixed << setprecision(2) << acumTotal << " Bs." << endl
         << "Total IVA recaudado:       " << acumImpuestos << " Bs." << endl
         << "Unidades totales vendidas por producto:" << endl;

    // Va a imprimir todos los productos y cuatas unidades se han vendido
    for (size_t i = 0; i < inventario.size(); i++)
    {
        cout << "- " << inventario[i].nombre << ": " << inventario[i].vendidos << " unidades." << endl;
    }

    system("pause");
}

void cierreDia()
{
    int confCierre;
    // marco
    cout << "             CIERRE DEL DÍA                  " << endl;

    cout << "Transacciones totales del turno: " << conVentas << endl;
    cout << "Ingresos generados: " << fixed << setprecision(2) << acumTotal << " Bs." << endl;
    cout << "---------------------------------------------" << endl;
    cout << "¿Desea blanquear los contadores y cajas?\n1) Sí, reiniciar\n2) No, conservar datos\nOpción: ";
    cin >> confCierre;

    // Vacia todo
    if (confCierre == 1)
    {
        conVentas = 0;
        acumTotal = 0;
        acumImpuestos = 0;
        venEfectivo = 0;
        venTarjeta = 0;
        venTransferencia = 0;
        cout << "\nCaja diaria reiniciada exitosamente." << endl;
    }
    // Deja los datos actuales
    else
    {
        cout << "\nDatos del turno conservados en memoria." << endl;
    }
    system("pause");
}

void actualizarTasaDolar()
{
    double nuevaTasa;
    // marco
    cout << "        ACTUALIZACIÓN DE TASA (BCV)          " << endl;
    cout << "Tasa actual registrada: " << tasaCambio << " Bs." << endl;
    cout << "Ingrese la nueva tasa oficial del mercado: ";
    cin >> nuevaTasa;

    if (nuevaTasa > 0)
    {
        tasaCambio = nuevaTasa;
        cout << "\nTasa del dólar ajustada con éxito a " << tasaCambio << " Bs." << endl;
        cout << "Los precios de la base de datos se reajustaron automáticamente." << endl;
    }
    else
    {
        cout << "\nError: La tasa debe ser mayor a 0." << endl;
    }
    system("pause");
}
