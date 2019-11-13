#include "Busqueda.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <dirent.h>
#include <algorithm>
using namespace std;
Busqueda::Busqueda(void) :Primero(nullptr), Ultimo(nullptr),Auxiliar(nullptr) { }

bool Busqueda::estaVacia() {
	return Primero == 0;
}

void Busqueda::ExtraerDatos(){
	DIR* dirp = opendir(direccion.c_str());
	struct dirent* dp;
	while ((dp = readdir(dirp)) != NULL){
			CargarDatos(dp->d_name);//aqui guardo los datos en un arreglo de objetos
			contadorDocumentos++;
	}
	closedir(dirp);
	contarExtraer++;
	contadorDocumentos = contadorDocumentos - 2;
}

void Busqueda::Buscar(std::string _clave) {
	int casoBase = 1000;
	//int Encontrado = 0;
	//if (contarExtraer == 0) {
	//	ExtraerDatos();
	//	contarExtraer++;
	//}
	ExtraerDatos();
	if (_clave.size() > 3) {
		std::size_t encontrar;
		for (int i = 0; i < contadorDocumentos; i++) {
			encontrar = Paginas[i].Keywords.find(_clave);//voy a mandar esa clave una funcion para saber si ya esta el nodo
			if (encontrar < std::string::npos) { //si esta en el keyword esa palabra incluyo esa posicion para posiblemente agregarlo
				InsertarNodoAbajo(i);
				if (Paginas[i].cantidadReferencias > 0) {
					for (int j = 0; j < Paginas[i].cantidadReferencias; j++)
					{
						InsertarNodoSiguiente(Paginas[i].Referencias[j]);
					}
				}
			}
		}
		buscarPorImportancia();
		PageRank(casoBase);
		//Ordenar();
		ImprimirNodos();
		//EliminarNodo();
		GuardarArchivo();
	}
}

	
void Busqueda::buscarPorImportancia(){
	NodoPagina* actual = Primero;
	std::string::size_type pos;
	char* nombre;
	while (actual != nullptr) {
		if (actual->cantidadReferencias > 0) {
			for (int i = 0; i < contadorDocumentos; i++)
			{
				if (strcmp(Paginas[i].Nombre, actual->Nombre) == 0) {
						for (int j = 0; j < Paginas[i].cantidadReferencias; j++)
						{
							for (int k = 0; k < contadorDocumentos; k++)
							{
								pos = Paginas[i].ReferenciasB[j].find(Paginas[k].Nombre);
								if (pos<std::string::npos && !siExiste(k)) {
									InsertarNodoAbajo(k);
									if (Paginas[k].cantidadReferencias > 0) {
										for (int l = 0; l < Paginas[k].cantidadReferencias; l++)
										{
											InsertarNodoSiguiente(Paginas[k].Referencias[l]);
										}
									}
								}
							}
						}
				}
			}
		}
		actual = actual->abajo;
	}
}


bool Busqueda::siExiste(int posicion) {
	NodoPagina* actual = Primero;
	if (Paginas[posicion].Ingresado) {
		return true;
	}
	return false;
}

void Busqueda::InsertarNodoAbajo(int posicionA) {//primero debe ser nullptr para modificar el nodo
	NodoPagina* nuevo=new NodoPagina();
	NodoPagina* actual = Primero;
	nuevo->Nombre = Paginas[posicionA].Nombre;
	nuevo->pagerank = Paginas[posicionA].PageRank;//aqui voy a mandar el pagerank de la pagina
	nuevo->cantidadReferencias = Paginas[posicionA].cantidadReferencias;
	Paginas[posicionA].Ingresado = true;
	if (estaVacia()){
		Primero = nuevo;
		Ultimo = nuevo;
	}
	else {
		Ultimo->abajo = nuevo;
		Ultimo = nuevo;
	}
}

void Busqueda::InsertarNodoSiguiente(char* _nombre) {//primero debe ser nullptr para modificar el nodo
	NodoPaginaHref* nuevo = new NodoPaginaHref();
	nuevo->Nombre = _nombre;
	if (Ultimo->siguiente==nullptr){
		Ultimo->siguiente = nuevo;
		Auxiliar = nuevo;
	}
	else {
		Auxiliar->siguiente = nuevo;
		Auxiliar = nuevo;
	}
}

void Busqueda::EliminarNodo() {
	NodoPagina* actual = Primero;
	NodoPagina* ContadorNodosA = Primero;
	NodoPagina* aEliminarAbajo;
	NodoPaginaHref* aEliminarS;
	int contador = 0;
	int cantidadReferencias = 0;
	while (ContadorNodosA!=nullptr) {//aqui el ContadorNodosA cuenta la cantidad de nodos
		contador++;
		ContadorNodosA = ContadorNodosA->abajo;
	}
	if (contador > 0) {//si en la busqueda agregue nodos entonces los borro
		cantidadReferencias = actual->cantidadReferencias;
		for (int i = 0; i < contador; i++)//hago un ciclo for para borrar segun la cantidad de nodos que tengo hacia abajo
		{

			if (cantidadReferencias > 0){//si el nodo tiene referencias entonces las borro
				while (actual->siguiente != nullptr){
					aEliminarS = actual->siguiente;
					actual->siguiente = aEliminarS->siguiente;//actual va a apuntar al que apunta el que estoy eliminando
					delete(aEliminarS);
				}
				Primero = actual->abajo;//el primero sera el siguiente del actual 
				aEliminarAbajo = actual;
				actual = actual->abajo;
				delete(aEliminarAbajo);
			}else {
				Primero = actual->abajo;//el primero sera el siguiente del actual 
				aEliminarAbajo = actual;
				actual = actual->abajo;
				delete(aEliminarAbajo);
			}
		}
	}
}

void Busqueda::ImprimirNodos() {
	NodoPagina* actual = Primero;
	NodoPaginaHref* actualDerecha;
	if (estaVacia()) {
		return;
	}
	while (actual!= nullptr) {
		actualDerecha = actual->siguiente;
		std::cout << "<p><span style= ´font-weight´: bold´> "<<"Pagina: "<<actual->Nombre << "<br>";
		std::cout <<"PageRank: "<< actual->pagerank << "<br>";
		std::cout <<"Cantidad de Referencias: "<< actual->cantidadReferencias << "<br>";
			while (actualDerecha != nullptr){
				std::cout << "<p><span style= ´font-weight´: bold´>" << "Referencia: " 
					<< " <a href=" << actualDerecha->Nombre << ">" << actualDerecha->Nombre << 
					"</a> " <<"</span>"<<"<br>";
				actualDerecha = actualDerecha->siguiente;
			}
			std::cout << "<br>";
		actual = actual->abajo;
	}
}

//void Busqueda::ImprimirNodos() {
//	NodoPagina* actual = Primero;
//	NodoPaginaHref* actualDerecha;
//	if (estaVacia()) {
//		return;
//	}
//	while (actual != nullptr) {
//		actualDerecha = actual->siguiente;
//		std::cout << "Pagina: " << actual->Nombre <<"\n";
//		std::cout << "PageRank: " << actual->pagerank<< "\n";
//		std::cout << "Cantidad de Referencias: " << actual->cantidadReferencias << "\n";
//		if (actual->cantidadReferencias > 0) {
//			while (actualDerecha != nullptr) {
//				std::cout << "Referencia: " << actualDerecha->Nombre << "\n";
//				actualDerecha = actualDerecha->siguiente;
//			}
//		}
//		std::cout << "\n";
//		actual = actual->abajo;
//	}
//}

void Busqueda::GuardarArchivo(){
	int contador = 0;
	DIR* dirp = opendir("C:/xampp/htdocs/historial/");
	struct dirent* dp;
	while ((dp = readdir(dirp)) != NULL) {
		contador++;
	}
	closedir(dirp);
	contador=contador - 1;
	std::string contadorS = std::to_string(contador);
	std::string direccion = "C:/xampp/htdocs/historial/historial"+contadorS+".html";
	std::string nombreA="C:/xampp/htdocs/historial/historial" + contadorS;
	NodoPagina* actual = Primero;
	NodoPaginaHref* actualDerecha;
	std::ofstream Archivo(direccion, std::ios::beg);
	Archivo << " <!DOCTYPE html>\n";
	Archivo << "<html>"<<"<br>";
	Archivo << "<head>" << "<br>";
	Archivo << "<title>"<<nombreA<<"</title>"<<"<br>";
	Archivo << "</head>"<<"<br>";
	Archivo << "<body>"<<"<br>";
	Archivo << "<div style=\"text-align: center; color: rgb(51, 51, 255);\">"<<"<br>";
	Archivo<<"<h1>"<<nombreA<<"</h1>"<<"<br>";
	Archivo << "</div>"<<"<br>";
	Archivo << "<p>"<<"<br>";
	while (actual != nullptr) {
		actualDerecha = actual->siguiente;
		Archivo << "Pagina:" << actual->Nombre<<"<br>";
		Archivo << "PageRank: " << actual->pagerank << "<br>";
		Archivo << "Cantidad de Referencias: " << actual->cantidadReferencias << "<br>";
		while (actualDerecha != nullptr) {
			Archivo <<  "Referencia: "<< " <a href=" << actualDerecha->Nombre << 
				">" << actualDerecha->Nombre << "</a>" << "</span>" << "<br>";
			actualDerecha = actualDerecha->siguiente;
		}
		Archivo << "</p>"<<"<br>";
		actual = actual->abajo;
	}
	Archivo << "</body>"<<"br";
	Archivo << "</html>"<<"br";
	Archivo.close();
}

void Busqueda::CargarDatos(std::string nombreDocumento) {
	std::string D = "C:/xampp/htdocs/recursos/";
	std::string direccionC = direccion + nombreDocumento;
	if (nombreDocumento.size() > 2) {
		Paginas[ContadorArreglo].Nombre = Nombre(direccionC.c_str());
		Paginas[ContadorArreglo].Keywords = Keyword(direccionC.c_str());
		Paginas[ContadorArreglo].cantidadReferencias = ContarReferencias(direccionC.c_str());
		Paginas[ContadorArreglo].PageRank = 0;
		for (int i = 0; i < Paginas[ContadorArreglo].cantidadReferencias; i++)
		{
			Paginas[ContadorArreglo].Referencias.push_back(Referencia(direccionC.c_str(), ContadorArreglo)[i]);
		}
		for (int i = 0; i < Paginas[ContadorArreglo].cantidadReferencias; i++)
		{
			Paginas[ContadorArreglo].ReferenciasB.push_back(ReferenciaB(direccionC.c_str(), ContadorArreglo)[i]);
		}
		ContadorArreglo++;
	}
}

void Busqueda::ImprimirDatos() {
	for (int i = 0; i < contadorDocumentos; i++)
	{
		std::cout << Paginas[i].Nombre << "\n";
		std::cout << Paginas[i].Keywords << "\n";
		std::cout << Paginas[i].cantidadReferencias << "\n";
		std::cout << Paginas[i].Keywords << "\n";
		for (size_t  j= 0; j < Paginas[i].Referencias.size(); j++)
		{
			std::cout << Paginas[i].Referencias[j]<<"\n";
		}
		for (size_t j = 0; j < Paginas[i].Referencias.size(); j++)
		{
			std::cout << Paginas[i].ReferenciasB[j] << "\n";
		}
	}
}

std::string Busqueda::Keyword(std::string nombreP) {
	std::string pagina;
	std::ifstream Archivo(nombreP.c_str(), std::ios::app);
	std::string::size_type pos;
	if (!Archivo) {
		std::cout << "Error al abrir archivo";
		return " " ;
	}
	while (std::getline(Archivo, pagina)) {
		pos = pagina.find("keywords");
		if (pos < std::string::npos){
			return ReducirKeyword(pagina);
		}
	}
}

char* Busqueda::Nombre(std::string nombreP) {
	std::string pagina;
	std::ifstream Archivo(nombreP.c_str(), std::ios::app);
	std::string::size_type pos;
	
	if (!Archivo) {
		std::cout << "Error al abrir archivo";
	}
	while (std::getline(Archivo,pagina)) {
		pos = pagina.find("<title>");
		if (pos < std::string::npos) {
			return ReducirNombre(pagina);
		}
	}
}

char* Busqueda::ReducirNombre(std::string pagina) {
	char* auxiliar=new char[pagina.length()];
	std::string::size_type pos = pagina.find("<title>");
	if (pos < std::string::npos) {
		pagina.replace(pos, std::string("<title>").length(), "");
	}
	std::string::size_type pos2 = pagina.find("</title>");
	if (pos2 < std::string::npos) {
		pagina.replace(pos2, std::string("</title>").length(), "");
	}
	strcpy_s(auxiliar,30, pagina.c_str());
	return auxiliar;
}

std::string Busqueda::ReducirKeyword(std::string pagina) {
	std::string::size_type pos = pagina.find("<meta name=\"keywords\" content=\"");
	if (pos < std::string::npos) {
		pagina.replace(pos, std::string("<meta name=\"keywords\" content=\"").length(), "");
	}
	std::string::size_type pos2 = pagina.find("\">");
	if (pos2 < std::string::npos) {
		pagina.replace(pos2, std::string("\">").length(), "");
	}
	//std::string::size_type pos3;
	//for (int i = 0; i < 60; i++)
	//{
	//	std::string::size_type pos3 = pagina.find("\n");
	//	if (pos2 < std::string::npos) {
	//		pagina.replace(pos2, std::string("\n").length(), "-");
	//	}
	//}
	return pagina;
}

std::vector<char*> Busqueda::Referencia(std::string nombreP, int posicion) {
	std::vector<char*> arreglo;
	std::string pagina;
	std::string auxiliar;
	int contador = 0;
	std::string::size_type pos;
	std::ifstream Archivo(nombreP.c_str(), std::ios::app);
	if (!Archivo) {
		std::cout << "Error al abrir archivo";
	}
	while (std::getline(Archivo, pagina)) {
		pos = pagina.find("href");
		if (pos < std::string::npos) {
			 arreglo.push_back(ReducirReferencia(pagina));
		}
	}
	return arreglo;
}

char* Busqueda::ReducirReferencia(std::string pagina) {
	char* auxiliarChar= new char[pagina.length()];
		std::string::size_type pos = pagina.find("<li><a href=\"");
		if (pos < std::string::npos) {
			pagina.replace(pos, std::string("<li><a href=\"").length(), "");
		}
		std::string::size_type pos2 = pagina.find("</a></li>");
		if (pos2 < std::string::npos) {
			pagina.replace(pos2, std::string("</a></li>").length(), "");
		}
		std::string::size_type pos3 = pagina.find(">");
		if (pos3 < std::string::npos) {
			pagina.replace(pos3, std::string(">").length(), "");
		}
		std::string auxiliar;
		std::string::size_type pos4= pagina.find("\"");
		for (size_t i = 0; i < pos4; i++)
		{
			auxiliar+=pagina[i];
		}


		strcpy_s(auxiliarChar,30,auxiliar.c_str());
		return auxiliarChar;
}

std::vector<std::string> Busqueda::ReferenciaB(std::string nombreP, int posicion) {
	std::vector<std::string> arreglo;
	std::string pagina;
	std::string auxiliar;
	int contador = 0;
	std::string::size_type pos;
	std::ifstream Archivo(nombreP.c_str(), std::ios::app);
	if (!Archivo) {
		std::cout << "Error al abrir archivo";
	}
	while (std::getline(Archivo, pagina)) {
		pos = pagina.find("href");
		if (pos < std::string::npos) {
			arreglo.push_back(ReducirReferenciaB(pagina));
		}
	}
	return arreglo;
}

std::string Busqueda::ReducirReferenciaB(std::string pagina) {
	std::string::size_type pos = pagina.find("<li><a href=\"");
	if (pos < std::string::npos) {
		pagina.replace(pos, std::string("<li><a href=\"").length(), " ");
	}
	std::string::size_type pos2 = pagina.find("</a></li>");
	if (pos2 < std::string::npos) {
		pagina.replace(pos2, std::string("</a></li>").length(), " ");
	}
	std::string::size_type pos3 = pagina.find(">");
	if (pos3 < std::string::npos) {
		pagina.replace(pos3, std::string(">").length(), " ");
	}
	std::string auxiliar;
	std::string::size_type pos4 = pagina.find("\"");//si encuentro la comilla
	for (size_t i = 0; i < pos4; i++)
	{
		auxiliar += pagina[i];
	}
	std::string cadena=auxiliar;
	int espacio;
	int aux;
	for(int i = 0; i < cadena.length(); i++)
	{
		if (cadena[i] == 32){
			espacio = i;
			aux = i;
			while (cadena[aux] == 32 && aux < (cadena.length() - 1)) aux++;
				cadena[espacio] = cadena[aux];
				cadena[aux] = 32;
		}
	}
	return cadena;
}

int Busqueda::ContarReferencias(std::string nombreP) {
	std::string pagina;
	int contador = 0;
	std::string::size_type pos;
	std::ifstream Archivo(nombreP.c_str(), std::ios::app);
	if (!Archivo) {
		std::cout << "Error al abrir archivo";
		return 0;
	}
	while (std::getline(Archivo, pagina)) {
		pos = pagina.find("href");
		 if (pos < std::string::npos) {
			contador++;
		}
	}
	return contador;
}

void Busqueda::PageRank(int casoBase) {
	if (casoBase>0){
		BuscarPageRank();
		GuardarPageRank();//aqui guardo el pagerank en cada iteracion
		PageRank(casoBase - 1);
	} else{
		return;
	}
}

void Busqueda::BuscarPageRank() { //esto va a ir dentro de un for para hacer las iteraciones de todas las paginas
	NodoPagina* actual = Primero;
	while (actual != nullptr) {
		for (int i = 0; i < contadorDocumentos; i++)
		{
			if (strcmp(actual->Nombre, Paginas[i].Nombre) == 0) {//aqui busco el nombre en el arreglo para calcular el page
				actual->pagerank = calcularPageRank(i);
			}
		}
		actual = actual->abajo;
	}
}

double Busqueda::calcularPageRank(int _posicion) {//aqui es donde esta mi error
	double PageRankReferencias = 0;
	std::string::size_type pos;
	for (int i = 0; i < contadorDocumentos; i++)
	{
		if (Paginas[i].cantidadReferencias > 0) {
			for (int j = 0; j < Paginas[i].cantidadReferencias; j++)
			{
				pos = Paginas[i].ReferenciasB[j].find(Paginas[_posicion].Nombre);
				if (pos < std::string::npos) {
					PageRankReferencias += ((Paginas[i].PageRank) / (Paginas[i].cantidadReferencias));
				}
			}
		}
	}
return  Paginas[_posicion].PageRank=(1 - 0.85) + ((0.85) * (PageRankReferencias));;
}

void Busqueda::GuardarPageRank(){
	NodoPagina* actual = Primero;
	while (actual != nullptr) {
		for (int i = 0; i < contadorDocumentos; i++)
		{
			if (strcmp(Paginas[i].Nombre, actual->Nombre) == 0) {
				Paginas[i].PageRank = actual->pagerank;
			}
		}
		actual = actual->abajo;
	}
}

//void Busqueda::Ordenar() {
//	NodoPagina** inicio = 0;
//	NodoPagina* actual = Primero;
//	NodoPagina* abajo;
//	NodoPagina* MasAbajo;
//	if (estaVacia()) {
//		return;
//	}
//	while (!estaOrdenado()) {
//		if (actual->pagerank < actual->abajo->pagerank && actual->abajo != nullptr) {
//			if (actual == Primero) {
//				abajo = actual->abajo;
//				MasAbajo = actual->abajo->abajo;
//				Primero = abajo;
//				abajo->abajo = actual;
//				actual->abajo = MasAbajo;
//			}
//			else if (actual->abajo->abajo == nullptr) {
//				abajo = actual->abajo;
//				MasAbajo == nullptr;
//				abajo->abajo = actual;
//				actual->abajo = nullptr;
//			}
//			else {
//				abajo = actual->abajo;
//				MasAbajo = actual->abajo->abajo;
//				abajo->abajo = actual;
//				actual->abajo = MasAbajo;
//			}
//		}else if (actual->pagerank >= actual->abajo->pagerank) {
//			actual = actual->abajo;
//		}
//		if (actual == Ultimo) {
//			abajo = nullptr;
//			MasAbajo = nullptr;
//			actual->abajo = nullptr;
//			actual = Primero;
//		}
//	}
//}


//
//void Busqueda::Ordenar() {
//	NodoPagina* actual = Primero;
//	NodoPagina* abajo;
//	NodoPagina* MasAbajo;
//	if (estaVacia()) {
//		return;
//	}
//	while (!estaOrdenado()){
//		if (actual->pagerank < actual->abajo->pagerank && actual->abajo != nullptr) {
//			if (actual == Primero){
//				abajo = actual->abajo;
//				MasAbajo = actual->abajo->abajo;
//				abajo->abajo = actual;
//				actual->abajo = MasAbajo;
//				Primero = abajo;
//			}else if (actual->abajo->abajo == nullptr) {
//				abajo = actual->abajo;
//				MasAbajo = actual->abajo->abajo;
//				abajo->siguiente = abajo->siguiente;
//				abajo->abajo = actual;
//				actual->abajo = MasAbajo;
//			}else  {
//				abajo = actual->abajo;
//				MasAbajo = actual->abajo->abajo;
//				abajo->abajo = actual;
//				actual->abajo = MasAbajo;
//			}
//		}else if (actual->pagerank >= actual->abajo->pagerank) {
//			actual = actual->abajo;
//		}
//			if (actual == Ultimo) {//si llego a mi ultimo
//				abajo = nullptr;
//				MasAbajo = nullptr;
//				actual->abajo = nullptr;
//				actual = Primero;
//		}
//	}//termina el while
//}

bool Busqueda::estaOrdenado(){
	NodoPagina* actual = Primero;
	for (int i = 0; i < 100000; i++)
	{
		actual = Primero;
			while (actual != nullptr && actual->abajo!=nullptr) {
				if (actual->pagerank < actual->abajo->pagerank) {
					return false;;
				}
				actual = actual->abajo;
		}
	}
		return true;
}

void Busqueda::Ordenar() {
	std::vector<NodoPagina*>Pr;
	NodoPagina* actual = Primero;
	while (actual!=0)
	{
		Pr.push_back(actual);
		actual = actual->abajo;
	}
	sort(Pr.begin(), Pr.end(), Xgreater());
}

