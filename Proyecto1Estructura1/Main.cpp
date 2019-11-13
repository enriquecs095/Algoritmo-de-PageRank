#include "Busqueda.h"
#include <iostream>
#include <string>
int main(int argc, char** argv) {
	Busqueda busqueda; 
	busqueda.Buscar(argv[1]);//aqui empieza lo bueno

}

//#include "Busqueda.h"
//#include <iostream>
//#include <string>
//
//int main() {
//	int opcion;
//	bool salir = false;
//	bool salirP = false;
//	int guardar;
//	std::string palabraBuscar;
//	Busqueda busqueda;
//	busqueda.ExtraerDatos();
//	int casoBase = 1000;
//	do {
//		std::cout << "\t\t\"BUSCADOR CRUZ\"\n";
//		std::cout << "Encuentra toda la informacion que necesitas entre ellos\nmusica, videos y libros, " <<
//			"accede a las redes sociales mas\nfamosas de la web. ";
//		std::cout << "\nCrea tu pagina, nosotro nos encargamos\nde la publicidad\n";
//		std::cout << "1. Buscar\n";
//		std::cout << "2.Salir\n";
//		std::cin >> opcion;
//		switch (opcion) {
//		case 1:
//			std::cout << "Ingrese la palabra clave: ";
//			std::cin.ignore();
//			std::getline(std::cin, palabraBuscar);
//			if (!palabraBuscar.empty()){
//				if (palabraBuscar.length() > 3) {
//					busqueda.Buscar(palabraBuscar);
//					busqueda.buscarPorImportancia();
//					busqueda.PageRank(casoBase);
//					busqueda.Ordenar();
//					busqueda.ImprimirNodos();
//					busqueda.GuardarArchivo();
//					busqueda.EliminarNodo();
//				}
//			}
//			break;
//
//		case 2:
//			salir = true;
//			break;
//
//		default:
//			std::cout << "Opcion incorrecta\n";
//			break;
//		}
//		std::cout << "\n";
//	} while (!salir);
//}