#pragma once
//#pragma warning(push)
//#pragma warning(disable: warning-code) //4996 for _CRT_SECURE_NO_WARNINGS equivalent
////deprecated code here
//#pragma warning(pop)
#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

struct Keywords{
	char* Nombre;
	std::string Keywords;
	int cantidadReferencias;
	double PageRank;
	bool Ingresado = false;
	std::vector<char*> Referencias;
	std::vector<std::string> ReferenciasB;
};

struct NodoPaginaHref{
	char* Nombre;
	NodoPaginaHref* siguiente;
	NodoPaginaHref(){
		siguiente = 0;
	}
};


struct NodoPagina {
	char* Nombre;
	double pagerank;
	NodoPagina* abajo;
	NodoPaginaHref* siguiente;
	int cantidadReferencias;

	NodoPagina() {
		abajo = 0;
		siguiente = 0;
	}
};
struct Xgreater {
	bool operator()(const NodoPagina* lx, const NodoPagina* rx) {
		return lx->pagerank > rx->pagerank;
	}
};

class Busqueda{

private:
	NodoPagina* Primero;
	NodoPagina* Ultimo;
	NodoPaginaHref* Auxiliar;
	Keywords Paginas[20];
	int ContadorArreglo=0;
	int contadorDocumentos=0;
	int contarExtraer = 0;
	std::string direccion = "C:/Users/Enrique/Documents/recursos/" ;

public:
	Busqueda(void);
	bool estaVacia();
	bool siExiste(int);
	void Buscar(std::string);
	void CargarDatos(std::string);
	int PaginasSize() {return 20;}
	void InsertarNodoAbajo(int);
	void InsertarNodoSiguiente(char*);
	int ContarReferencias(std::string);
	std::string Keyword(std::string);
	std::string ReducirKeyword(std::string);
	std::vector<char*> Referencia(std::string,int);
	char* ReducirReferencia(std::string);
	std::vector<std::string> ReferenciaB(std::string, int);
	std::string ReducirReferenciaB(std::string);
	char* Nombre(std::string);
	char* ReducirNombre(std::string);
	void ExtraerDatos();
	void ImprimirNodos();
	void EliminarNodo();
	void ImprimirDatos();
	void PageRank(int);
	void GuardarPageRank();
	void BuscarPageRank();
	double calcularPageRank(int);
	void GuardarArchivo();
	void Ordenar();
	bool estaOrdenado();
	void buscarPorImportancia();

};

#endif

