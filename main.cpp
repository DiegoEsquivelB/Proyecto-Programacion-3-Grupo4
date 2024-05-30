#include <iostream>
#include <string>
#include <curl/curl.h>
#include <stdlib.h>
#include <fstream>
#include <cstdlib> // Para la función system()
#include <map>
#include <windows.h>
#include <direct.h>

#define usuarioAdmin "administrador"
#define pass "adm"

using namespace std; //OJO CON ESTO

// Function to capture the response data from the HTTP request
size_t WriteCallback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}












void ConecAPI();
void cifrarCred(string palabra, string &nuevapalabra);
void crearCredenciales(string, string);
void verificarCreedenciales(string, string, string &, bool &);
void tamtxt(int &,string);
void MostrarHistorial(string,string);
void palabrasMasBuscadas(string idioma,string usuario);
void lecturaUMG(string frase1,string frase2, string idOr, string idFn,string usuario);
void codificacion(string frase);
void GenerarHistorial(string frase1,string frase2,string idOr, string idFn,string usuario);
void menuIngresar(string &usuario, string &contra);
void menuPrincipal(int &opc,string usuario);
void Submenu1(int &opc, string texto, string lenguaje);
void ReproducirSonido(string textoTrad, string idioma);
int opc;
string usuario;
string contra;
int reproducir;
string usuarioref;
bool salir=false;

int main() {
	
menuIngresar(usuario,contra);




menuPrincipal(opc,usuario);

/*
if(usuarioAdmin==usuario&&pass==contra){
	cout<<"HAS INGRESADO CON CONTROL MAESTRO"<<endl;
	system("pause");
	system("cls");
	menuPrincipal(opc);	
}else{
	cout<<"Usuario y/o clave incorrectos"<<endl;
	system("pause");
	system("cls");
}*/

	




	
	system("cls");
    return 0;
}

void ConecAPI(){
	int opc;
	do {
		string frase1, lenguajeInicial, lenguajeFinal, textoTrad;
	cout<<"\n----TRADUCIR PALABRAS----";
	cout<<"\n--Ingresa la palabra o texto--: " ;
	cin.ignore();
	std::getline(std::cin, frase1);
	
	cout<<"\n|Espanol (es), Ingles (en), Italiano (it), Frances (fr), Aleman (de)|"<<endl;
	cout<<"--Ingresa el lenguaje inicial--"<<endl;
	cin>>lenguajeInicial;
	cout<<"--Ingresa el lenguaje final--"<<endl;
	cin>>lenguajeFinal;

	

    // Create a libcurl easy handle for the request
    CURL *curl = curl_easy_init();

    // Check if the handle was created successfully
    if (curl) {
        // Set the URL for the Google Translate API endpoint
        curl_easy_setopt(curl, CURLOPT_URL, "https://translation.googleapis.com/language/translate/v2");
        curl_easy_setopt(curl, CURLOPT_POST, 1);

        // Construct the request body in JSON format
        std::string body = "{";
        body += "\"q\": \"" + frase1 + "\",";
        body += "\"source\": \"" + lenguajeInicial + "\",";
        body += "\"target\": \"" + lenguajeFinal + "\"";
        body += "}";

        // Set the request body
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

        // Set the request headers
        struct curl_slist *headers = NULL;
       headers = curl_slist_append(headers, "CLAVE API ACA");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up a custom write function to capture the response data
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Disable SSL certificate verification for simplicity (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Perform the HTTP request
        CURLcode res = curl_easy_perform(curl);

        // Check if the request was successful and extract the translated text
        if (res == CURLE_OK) {
            // Find the index of the start and end of the translated text
            size_t start = response.find("\"translatedText\":") + 17; // 17 is the length of "\"translatedText\": \""
           // size_t end = response.find("\",", start);
           // size_t end = response.find("\"}", start);
          // size_t end = response.find("\"\n", start);
          size_t end = response.find_first_of("\n", start);

            // Extract the translated text substring
            std::string translatedText = response.substr(start, end - start);		
            std::cout << "El texto traducido es: " << translatedText << std::endl;
        	
        	string frase2;
        	
         for (size_t i = 0; i < translatedText.length(); ++i) {
        		char c = translatedText[i];
        if (c != '\"') { // Si el carácter no es una comilla doble
            frase2 += c; // Lo añadimos a la nueva cadena
        }
    }
    
 
    frase2.erase(0, 1); // Borra el primer carácter de la cadena


    	GenerarHistorial(frase1,frase2, lenguajeInicial,lenguajeFinal,usuario);
        Submenu1(opc, frase2, lenguajeFinal);
            
            
        } else {
            std::cerr << "Error performing the request: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up libcurl resources
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Error initializing libcurl" << std::endl;
    }
	
	

		
	
		
} while(opc!=3);
	system("cls");
	menuPrincipal(opc,usuario);
	cin.ignore();
	
}

void GenerarHistorial(string frase1,string frase2,string idOr, string idFn,string usuario){
	ofstream historial1, historial2;
	
	    string directorio = "historial_" + usuario;

    
    if (!CreateDirectory(directorio.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            cerr << "Error al crear el directorio: " << directorio << endl;
            return;
        }
    }
	
	
	string nombreArchivo = directorio + "\\historialbusqueda_" + usuario + "-" + idOr + ".txt";
    string nombreArchivo2 = directorio + "\\historialbusqueda_" + usuario + "-" + idFn + ".txt";
	
	SetFileAttributes(nombreArchivo.c_str(), FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(nombreArchivo2.c_str(), FILE_ATTRIBUTE_NORMAL);
	
	
	historial1.open(nombreArchivo.c_str(),ios::out|ios::app);
	historial1<<frase1<<"\n";
	
	historial2.open(nombreArchivo2.c_str(),ios::out|ios::app);
	historial2<<frase2<<"\n";
	
	
	
					   if (!SetFileAttributes(nombreArchivo.c_str(), FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM)) {
		   	cout << "Error al cambiar los atributos del archivo." <<endl;
        return;
    }
    
    				   if (!SetFileAttributes(nombreArchivo2.c_str(), FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM)) {
		   	cout << "Error al cambiar los atributos del archivo." <<endl;
        return;
    }
	
	
	lecturaUMG(frase1,frase2,idOr,idFn,usuario);
	
	
	
}

void MostrarHistorial(string idioma,string usuario){
	ifstream archivo;
	string texto;
	int i=1;
	
	string directorio = "historial_" + usuario;

    
    if (!CreateDirectory(directorio.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            cerr << "Error al crear el directorio: " << directorio << endl;
            return;
        }
    }
	
	
	string nombrearchivo = directorio+"\\historialbusqueda_"+usuario+"-"+idioma+".txt";
	archivo.open(nombrearchivo.c_str(),ios::in);
	
	if(archivo.fail()){
		cout<<"No se encontro un historial"<<endl;
		system("pause");
		system("cls");
		int opc;
		menuPrincipal(opc,usuario);
	}else{
		cout<<"HISTORIAL DE BUSQUEDA DE: "+usuario<<endl;
			while(!archivo.eof()){
		getline(archivo,texto);
		cout<<i<<". "<<texto<<endl;
		i++;
	}
			system("pause");
		system("cls");	
	}
	

	archivo.close();
	
	
}

void lecturaUMG(string frase1,string frase2, string idOr, string idFn,string usuario){
	ifstream archivoClave;
	ofstream archivoCodificado;
	string clave;
	int tamTexto = frase1.size(); //longitud total de los caracteres de la frase
	int tamTexto2 = frase2.size();
	
	string cd[tamTexto]; //matriz donde se guardar la frace
	string cd2[tamTexto2];
	

	    string directorio = "historial_" + usuario;

    if (!CreateDirectory(directorio.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            cerr << "Error al crear el directorio: " << directorio << endl;
            return;
        }
    }

	
	
	string nombrearchivo= directorio+"\\archivocodificado_"+usuario+"-"+idOr+".txt";
	string nombrearchivo2= directorio+"\\archivocodificado_"+usuario+"-"+idFn+".txt";
	
	SetFileAttributes(nombrearchivo.c_str(), FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(nombrearchivo2.c_str(), FILE_ATTRIBUTE_NORMAL);
	
	
	archivoClave.open("umg.txt",ios::in); //abriendo el archivo modo lectura
	
	if(archivoClave.fail()){
		cout<<"No se a podido abrir el archivo"<<endl;
		exit(1);
	}
	
	
	while(!archivoClave.eof()){ // blucle que leera  todo el txt fila por fila
	getline(archivoClave,clave);
		for(int i=0; i<frase1.size();i++){ //bucle que recorrera caracter por caracter de toda la frase
			char c= frase1[i];  //varible donde se guardara cada caracter de la frase
				if(clave[0]==c){  //condicion donde comparara si el primer caracter de la clave a=u1 osea "a" sea igual a c que contiene los caracteres 
				cd[i]=clave; //si es igual entonces se inserta en la matriz la clave referente al caracter de la frase en esa posicion
				}			
		}
		
				for(int i=0; i<frase2.size();i++){
			char c= frase2[i];
				if(clave[0]==c){
				cd2[i]=clave;
				}			
		}

	}
	
	
	for(int i=0;i<tamTexto;i++){
			size_t pos = cd[i].find("=")+1; //busca la cantidad de caracteres que hay desde el inicio hasta que encuentre "=" y se le sumara uno para que se tome la posicion despues del caracter
				frase1.replace(0,tamTexto,cd[i],pos,cd[i].size()-pos);
				archivoCodificado.open(nombrearchivo.c_str(),ios::out|ios::app);
				archivoCodificado<<frase1;	
				archivoCodificado.close();	
	}
	
	
		archivoCodificado.open(nombrearchivo.c_str(),ios::app);
		archivoCodificado<<"\n";
		archivoCodificado.close();	
		archivoClave.close();
	
	
	
		for(int i=0;i<tamTexto2;i++){
			size_t pos = cd2[i].find("=")+1;
				frase2.replace(0,tamTexto2,cd2[i],pos,cd2[i].size()-pos);
				archivoCodificado.open(nombrearchivo2.c_str(),ios::out|ios::app);
				archivoCodificado<<frase2;	
				archivoCodificado.close();	
	}
	
	
		archivoCodificado.open(nombrearchivo2.c_str(),ios::app);
		archivoCodificado<<"\n";
		archivoCodificado.close();	
		archivoClave.close();
		
		
				   if (!SetFileAttributes(nombrearchivo.c_str(), FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM)) {
		   	cout << "Error al cambiar los atributos del archivo." <<endl;
        return;
    }
						   if (!SetFileAttributes(nombrearchivo2.c_str(), FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM)) {
		   	cout << "Error al cambiar los atributos del archivo." <<endl;
        return;
    }

	
	}

void cifrarCred(string palabra, string &nuevapalabra){

	size_t tam= palabra.size();
	string matr[tam];
	ifstream archivoClave;
	string clave;

	archivoClave.open("umg.txt",ios::in);
	
	if(archivoClave.fail()){
		cout<<"no se pudo abrir el archivo";
	}
	
	while(!archivoClave.eof()) {
		getline(archivoClave,clave);
		for(int i=0; i<palabra.size();i++){
			char c= palabra[i];
				if(clave[0]==c){
					matr[i]=clave;
				}
			}
		}
			for(int i=0;i<tam;i++){
				size_t posi = matr[i].find("=")+1;
				matr[i].replace(0,posi,"");
		}
		
		
		for(int i=0;i<tam;i++){
			nuevapalabra += matr[i];
		}
				
	archivoClave.close();
}

void palabrasMasBuscadas(string idioma,string usuario){
	ifstream archivo;
	string texto;
	
	
		    string directorio = "historial_" + usuario;

    
    if (!CreateDirectory(directorio.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            cerr << "Error al crear el directorio: " << directorio << endl;
            return;
        }
    }
	
	
	
	string nombrearchivo= directorio+"\\historialbusqueda_"+usuario+"-"+idioma+".txt";
	map<string,int>topPalabras;
	int index;
	
	archivo.open(nombrearchivo.c_str(),ios::in);

	
/*	while(!archivo.eof()){
	getline(archivo,texto);
	topPalabras.push_back(texto);
	}*/

if(archivo.fail()){
	int opc;
	cout<<"No se encontro un historial"<<endl;
	system("pause");
	system("cls");
	menuPrincipal(opc,usuario);
}else{
		index=topPalabras.size();
	
	    while (getline(archivo,texto)) {
        topPalabras[texto]++;
    }
	
    for (map<string,int>::iterator it = topPalabras.begin(); it != topPalabras.end(); ++it) {
                cout << it->first << " - " << it->second <<endl;
    }

		
}
			system("pause");
		system("cls");
}	

void tamtxt(int &cantidad,string nombrearchivo){
	ifstream archivo;
	int contador=0;
	string texto;
	
	archivo.open(nombrearchivo.c_str(),ios::in);
		while(!archivo.eof()){
		getline(archivo,texto);
		contador++;
		}
	cantidad= contador;
	archivo.close();
}

void menuIngresar(string &usuario, string &contra){
	
	do{
		cout<<"******MENU Ingresar******"<<endl;
	cout<<"----BIENVENIDOS AL TRADUCTOR----"<<endl;
	cout<<"1. Ingresar Usuario"<<endl;
	cout<<"2. Registrar Usuario"<<endl;
	cout<<"3. Salir"<<endl;
	cin>>opc;
	
	switch(opc) {
		case 1: {
			cout<<"Ingresa el usuario: ";
			cin>>usuario;
			cout<<"Ingresa la password: ";
			cin>>contra;
			verificarCreedenciales(usuario,contra,usuarioref,salir);
			if(salir){
				opc=3;
			}
			system("pause");
			break;
		
			
		}
		case 2:{
			
			cout<<"Ingresa el nuevo usuario: ";
			cin>>usuario;
			cout<<"Ingresa la password: ";
			cin>>contra;		
			crearCredenciales(usuario,contra);
		
			break;
			
			
			
			
		}
			case 3:{
				cout<<"GRACIAS POR USAR EL PROGRAMA"<<endl;
				system("pause");
				system("cls");
				break;
			}
	default:
		break;
}
		
		
	}while(opc!=3);

}

void menuPrincipal(int &opc,string usuario){
	
	
	do{
		cout<<"******MENU PRINCIPAL******"<<endl;
	cout<<"----BIENVENIDOS AL TRADUCTOR "+usuario+" ----"<<endl;
	cout<<"1. Traducir Palabras"<<endl;
	cout<<"2. Historial de busquedas"<<endl;
	cout<<"3. Palabras mas buscadas"<<endl;
	cout<<"4. Salir"<<endl;
	cout<<"Elige una opcion: ";
	cin>>opc;
	
	switch(opc) {
	case 1:{
		ConecAPI();
		break;
	}
	case 2:{
		system("cls");
		string resp;
		cout<<"Cual deseas consultar |Espanol (es), Ingles (en), Italiano (it), Frances (fr), Aleman (de)|"<<endl;
		cin>>resp;
		MostrarHistorial(resp,usuario);
		
		break;
	}	
	
	case 3:{
		system("cls");
		string resp;
		cout<<"Cual deseas consultar |Espanol (es), Ingles (en), Italiano (it), Frances (fr), Aleman (de)|"<<endl;
		cin>>resp;
		palabrasMasBuscadas(resp,usuario);
		break;
	}
	
	case 4:{
		break;
	}
	default:{
		break;
	}
}
		
		
	}while(opc!=4);
	
	
	
	
}

void Submenu1(int &opc, string texto, string lenguaje){
	cout<<"---------------Si(1), No(2)--------------------------"<<endl;
	cout<<"1. ¿DESEAS REPRODUCIR LA PALABRA O TEXTO?"<<endl;
	cout<<"2. ¿DESEAS INGRESAR OTRA PALABRA O TEXTO? "<<endl;
	cout<<"3. Regresar al menu principal"<<endl;
	cin	>>opc;
	
switch(opc) {
	case 1: {
		ReproducirSonido(texto, lenguaje);
		break;
	}
	case 2: {
		system("cls");
		ConecAPI();
	}
	
	default:
		break;
}
}

void ReproducirSonido(string texto, string lenguaje){
	
	cout<<texto;
    // Comando para llamar a espeak desde la línea de comandos con el idioma especificado
    std::string comando = "espeak -v " + lenguaje + " \"" + texto + "\" --stdout > output.wav";

    // Llamar a espeak para generar el archivo de audio
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        std::cout << "\nAudio generado exitosamente." << std::endl;
    } else {
        std::cerr << "Error al generar el audio." << std::endl;
    }
	
}

void verificarCreedenciales(string Usuario, string contra,string &usuarioref,bool &salir){
	
	ifstream archivo;
	string usuariocod, contracod;
	string nombrearchivo= "Credenciales_"+usuario+".txt";
	archivo.open(nombrearchivo.c_str(),ios::in);
	string matrizcred[2];
	int index=0;
	
	if(archivo.fail()){
		cout<<"NO SE ENCONTRO REGISTRO DE SUS CREDENCIALES"<<endl;
		system("pause");
		system("cls");
		
		return;
	}else{
		while(!archivo.eof()){
		getline(archivo,matrizcred[index]);
		index++;
	}

		
	
	
	cifrarCred(usuario,usuariocod);
	cifrarCred(contra,contracod);
	
	//cout<<usuariocod+" "+contracod<<endl;
		
	if(matrizcred[0]==usuariocod&&matrizcred[1]==contracod){
		cout<<"CREDENCIALES CORRECTAS"<<endl;
		usuarioref=Usuario;
		salir=true;
		system("pause");
		system("cls");
	}else{
		cout<<"CREDENCIALES INCORRECTAS"<<endl;
		system("pause");
		system("cls");
	}
		
	}
	
	
	
}

void crearCredenciales(string usuario, string contra){
	
	string nombrearchivo= "Credenciales_"+usuario+".txt";
	string usuariocod, contracod;
	
	cifrarCred(usuario,usuariocod);
	cifrarCred(contra,contracod);
	
	ifstream verificarexistencia(nombrearchivo.c_str());
	
	if(verificarexistencia){
		  cout << "El archivo de credenciales ya existe para este usuario." << endl;
		  verificarexistencia.close();
		  return;
	}
	
	cout << "Se creo con exito el usuario" << endl;
	ofstream archivo;
	archivo.open(nombrearchivo.c_str(),ios::trunc);	
	archivo<<usuariocod<<endl;
	archivo<<contracod<<endl;
	archivo.close();
	
	
		   if (!SetFileAttributes(nombrearchivo.c_str(), FILE_ATTRIBUTE_READONLY|FILE_ATTRIBUTE_SYSTEM)) {
		   	cout << "Error al cambiar los atributos del archivo." <<endl;
        return;
    }
	
	
	
	
	
	
	
 
	
}























