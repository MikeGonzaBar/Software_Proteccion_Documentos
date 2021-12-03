#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "sodium.h"
#include <fstream>
#include <string>

void menu();
void generacionClaves();
void cifradoArchivos();
void descifradoArchivos();
void firmaArchivos();
void verificacionFirmaArchivos();

unsigned char pk[crypto_sign_PUBLICKEYBYTES];
unsigned char sk[crypto_sign_SECRETKEYBYTES];

int main(){
	while (true) {
		if (sodium_init() < 0) {
			std::cout << "Error inesperado, favor de habilitar sodium" << std::endl;
			return 0;
		}
		menu();
	}
	return 0;
}

void menu() {
	int option;
	std::cout << "Software de Proteccion de Documentos" << std::endl << std::endl;
	std::cout << "1. Generacion de Claves hacia o desde 1 archivo" << std::endl;
	std::cout << "2. Cifrado de Archivos" << std::endl;
	std::cout << "3. Descifrado de Archivos" << std::endl;
	std::cout << "4. Firma de Archivos" << std::endl;
	std::cout << "5. Verificacion de Firma de Archivos" << std::endl<<std::endl;
	std::cout << "Selecciona una opcion: ";
	std::cin  >> option;
	system("cls");
	switch (option){
		case 1:
			generacionClaves();
			break;
		case 2:
			cifradoArchivos();
			break;
		case 3:
			descifradoArchivos();
			break;
		case 4:
			firmaArchivos();
			break;
		case 5:
			verificacionFirmaArchivos();
		default:
			break;
	}
}

void generacionClaves() {
	unsigned char pk[crypto_sign_PUBLICKEYBYTES];
	unsigned char sk[crypto_sign_SECRETKEYBYTES];
	crypto_sign_keypair(pk, sk);


	FILE* newf_docPK;
	std::string strPK("publicKey.txt");
	fopen_s(&newf_docPK, strPK.c_str(), "wb");
	fwrite(pk, 1, crypto_sign_PUBLICKEYBYTES, newf_docPK);
	fclose(newf_docPK);
	std::cout << std::endl;
	std::cout << "Public key guardada correctamente" << std::endl;
	system("pause");

	FILE* newf_docSK;
	std::string strSK("secretKey.txt");
	fopen_s(&newf_docSK, strSK.c_str(), "wb");
	fwrite(sk, 1, crypto_sign_SECRETKEYBYTES, newf_docSK);
	fclose(newf_docSK);
	std::cout << std::endl;
	std::cout << "Secret key guardada correctamente" << std::endl;
	system("pause");
	system("cls");
}

void cifradoArchivos() {
	unsigned char key[crypto_stream_chacha20_KEYBYTES];
	unsigned char nonce[crypto_stream_chacha20_NONCEBYTES];
	unsigned char* direction = new unsigned char[300];
	FILE* f_doc;

	//Lectura del archivo a cifrar
	std::cout << "Ingresa la direccion del archivo a cifrar: ";
	std::cin >> direction;
	std::cout << std::endl;
	fopen_s(&f_doc, reinterpret_cast<char*>(direction), "rb");
	fseek(f_doc, 0, SEEK_END);	//Ir al final del documento

	long fsize = ftell(f_doc);	//Tamaño del documento

	//Creación de variables donde se guardaran los mensajes
	unsigned char* document = new unsigned char[fsize];
	unsigned char* cipherText = new unsigned char[fsize];

	fseek(f_doc, 0, SEEK_SET);	//Ir al inicio del documento

	fread(document, 1, fsize, f_doc);
	fclose(f_doc);

	crypto_secretbox_keygen(key);			// Generar Key
	randombytes_buf(nonce, sizeof nonce);	//Generar Nonce


	int toCipher = crypto_stream_chacha20_xor(cipherText, document, fsize, nonce, key);

	FILE* newf_doc;
	std::string str("cyphered_");
	str.append(reinterpret_cast<char*>(direction));
	fopen_s(&newf_doc, str.c_str(), "wb");
	fwrite(cipherText, 1, fsize, newf_doc);
	fclose(newf_doc);
	std::cout << std::endl;
	std::cout << "Archivo cifrado correctamente" << std::endl;
	system("pause");


	FILE* newf_docK;
	std::string strK("key_");
	strK.append(reinterpret_cast<char*>(direction));
	fopen_s(&newf_docK, strK.c_str(), "wb");
	fwrite(key, 1, crypto_stream_chacha20_KEYBYTES, newf_docK);
	fclose(newf_docK);
	std::cout << std::endl;
	std::cout << "Llave guardada correctamente" << std::endl;
	system("pause");

	FILE* newf_docN;
	std::string strN("nonce_");
	strN.append(reinterpret_cast<char*>(direction));
	fopen_s(&newf_docN, strN.c_str(), "wb");
	fwrite(nonce, 1, crypto_stream_chacha20_NONCEBYTES, newf_docN);
	fclose(newf_docN);
	std::cout << std::endl;
	std::cout << "Nonce guardada correctamente" << std::endl;
	system("pause");
	system("cls");
}

void descifradoArchivos() {
	unsigned char* directionFile = new unsigned char[300];
	unsigned char* directionKey = new unsigned char[300];
	unsigned char* directionNonce = new unsigned char[300];
	std::cout << "Funcion de descifrado de archivos" << std::endl;
	std::cout << "Ingresa la direccion del archivo cifrado: ";
	std::cin >> directionFile;
	std::cout << "Ingresa la direccion del la llave: ";
	std::cin >> directionKey;
	std::cout << "Ingresa la direccion del nonce: ";
	std::cin >> directionNonce;

	//Lectura de archivo a Descifrar
	FILE* f_doc;
	fopen_s(&f_doc, reinterpret_cast<char*>(directionFile), "rb");
	fseek(f_doc, 0, SEEK_END);	//Ir al final del documento
	long fsize = ftell(f_doc);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* document = new unsigned char[fsize];
	unsigned char* decipherText = new unsigned char[fsize];
	fseek(f_doc, 0, SEEK_SET);	//Ir al inicio del documento
	fread(document, 1, fsize, f_doc);
	fclose(f_doc);

	std::cout << std::endl;

	//Lectura de archivo llave
	FILE* f_docK;
	fopen_s(&f_docK, reinterpret_cast<char*>(directionKey), "rb");
	fseek(f_docK, 0, SEEK_END);	//Ir al final del documento
	long fsizeK = ftell(f_docK);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* tempkey = new unsigned char[fsizeK];
	fseek(f_docK, 0, SEEK_SET);	//Ir al inicio del documento
	fread(tempkey, 1, fsizeK, f_docK);
	fclose(f_docK);
	

	//Lectura de nonce
	FILE* f_docN;
	fopen_s(&f_docN, reinterpret_cast<char*>(directionNonce), "rb");
	fseek(f_docN, 0, SEEK_END);	//Ir al final del documento
	long fsizeN= ftell(f_docN);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* tempnonce = new unsigned char[fsizeN];
	fseek(f_docN, 0, SEEK_SET);	//Ir al inicio del documento
	fread(tempnonce, 1, fsizeN, f_docN);
	fclose(f_docN);
	


	int toDecipher = crypto_stream_chacha20_xor(decipherText, document, fsize, tempnonce, tempkey);

	FILE* newf_doc;
	std::string str("decyphered_");
	str.append(reinterpret_cast<char*>(directionFile));
	fopen_s(&newf_doc, str.c_str(), "wb");
	fwrite(decipherText, 1, fsize, newf_doc);
	fclose(newf_doc);
	std::cout << std::endl;
	std::cout << "Archivo descifrado guardado correctamente" << std::endl;
	system("pause");
	system("cls");
	
}

void firmaArchivos() {
	unsigned char nonce[crypto_stream_chacha20_NONCEBYTES];
	std::cout << "Funcion de firmar archivos" << std::endl;
	
	unsigned char* directionFile = new unsigned char[300];
	unsigned char* directionSecretKey = new unsigned char[300];

	std::cout << "Ingresa la direccion del archivo a firmar: ";
	std::cin >> directionFile;
	std::cout << "Ingresa la direccion del la llave privada: ";
	std::cin >> directionSecretKey;

	//Lectura de archivo a firmar
	FILE* f_doc;
	fopen_s(&f_doc, reinterpret_cast<char*>(directionFile), "rb");
	fseek(f_doc, 0, SEEK_END);	//Ir al final del documento
	long fsize = ftell(f_doc);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* document = new unsigned char[fsize];
	fseek(f_doc, 0, SEEK_SET);	//Ir al inicio del documento
	fread(document, 1, fsize, f_doc);
	fclose(f_doc);

	std::cout << std::endl;
	system("pause");
	std::cout << std::endl;

	//Lectura de llave secreta
	FILE* f_docSK;
	fopen_s(&f_docSK, reinterpret_cast<char*>(directionSecretKey), "rb");
	fseek(f_docSK, 0, SEEK_END);	//Ir al final del documento
	long fsizeSK = ftell(f_docSK);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* skey = new unsigned char[fsizeSK];
	fseek(f_docSK, 0, SEEK_SET);	//Ir al inicio del documento
	fread(skey, 1, fsizeSK, f_docSK);
	fclose(f_docSK);

	std::cout << std::endl;
	system("pause");
	std::cout << std::endl;

	unsigned char* signed_message = new unsigned char[crypto_sign_BYTES + fsize];
	unsigned long long signed_message_len;

	crypto_sign(signed_message, &signed_message_len, document, fsize, skey);


	FILE* newf_doc;
	std::string str("signed_");
	str.append(reinterpret_cast<char*>(directionFile));
	fopen_s(&newf_doc, str.c_str(), "wb");
	fwrite(signed_message, 1, signed_message_len, newf_doc);
	fclose(newf_doc);
	std::cout << std::endl;
	std::cout << "Archivo firmado guardado correctamente" << std::endl;
	system("pause");
	system("cls");
	
}

void verificacionFirmaArchivos() {
	std::cout << "Funcion de verificacion de firma" << std::endl;

	unsigned char* directionFile = new unsigned char[300];
	unsigned char* directionPublicKey = new unsigned char[300];

	std::cout << "Ingresa la direccion del archivo a verificar: ";
	std::cin >> directionFile;
	std::cout << "Ingresa la direccion del la llave publica: ";
	std::cin >> directionPublicKey;

	//Lectura de archivo a firmar
	FILE* f_doc;
	fopen_s(&f_doc, reinterpret_cast<char*>(directionFile), "rb");
	fseek(f_doc, 0, SEEK_END);	//Ir al final del documento
	long fsize = ftell(f_doc);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* document = new unsigned char[fsize];
	fseek(f_doc, 0, SEEK_SET);	//Ir al inicio del documento
	fread(document, 1, fsize, f_doc);
	fclose(f_doc);

	system("pause");
	std::cout << std::endl;

	//Lectura de archivo llave pública
	FILE* f_docPK;
	fopen_s(&f_docPK, reinterpret_cast<char*>(directionPublicKey), "rb");
	fseek(f_docPK, 0, SEEK_END);	//Ir al final del documento
	long fsizePK = ftell(f_docPK);	//Tamaño del documento
	//Creación de variables donde se guardaran los mensajes
	unsigned char* pkey = new unsigned char[fsizePK];
	fseek(f_docPK, 0, SEEK_SET);	//Ir al inicio del documento
	fread(pkey, 1, fsizePK, f_docPK);
	fclose(f_docPK);

	system("pause");

	unsigned char* unsigned_message = new unsigned char [fsize- crypto_sign_BYTES];
	unsigned long long unsigned_message_len;

	if (crypto_sign_open(unsigned_message, &unsigned_message_len, document, fsize, pkey) != 0) {
		std::cout << "Firma no valida" << std::endl;
	}
	else {
		std::cout << "Firma valida" << std::endl;
		FILE* newf_doc;
		std::string str("unsigned_");
		str.append(reinterpret_cast<char*>(directionFile));
		fopen_s(&newf_doc, str.c_str(), "wb");
		fwrite(unsigned_message, 1, unsigned_message_len, newf_doc);
		fclose(newf_doc);
		std::cout << std::endl;
		std::cout << "Archivo sin firma guardado correctamente" << std::endl;
	}
	system("pause");
	system("cls");

}

