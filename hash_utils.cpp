#include "hash_utils.h"
#define NOMINMAX
#include <Windows.h>
//#include <bcrypt.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <openssl/des.h>
#include <iostream> 
#include <openssl/evp.h>
#include <algorithm> 
#include <cstring> 
#include <openssl/rand.h>

//#pragma comment(lib, "Bcrypt.lib")

/**
 * @brief Implementazione della funzione di hashing DES con ottimizzazioni
 * 
 * La funzione utilizza una cache thread-local per la key_schedule per 
 * migliorare le performance in caso di utilizzi ripetuti con lo stesso salt.
 * 
 * @details Il processo di hashing segue questi passaggi:
 * 1. Verifica/inizializza la key_schedule per il thread corrente
 * 2. Prepara l'input troncando la password a 8 byte se necessario
 * 3. Esegue la cifratura DES in modalità ECB
 * 4. Converte l'output in formato esadecimale
 */
std::string generateDESHash(const std::string& password, const std::string& salt) {
    // Cache della key_schedule per thread
    static thread_local DES_key_schedule key_schedule;
    static thread_local bool initialized = false;
    static thread_local std::string current_salt;

    // Inizializza o aggiorna la key_schedule solo se necessario
    if (!initialized || current_salt != salt) {
        DES_cblock key;
        memcpy(key, salt.c_str(), 8);  // Copia il salt nella chiave
        DES_set_key(&key, &key_schedule);  // Inizializza la key_schedule
        initialized = true;
        current_salt = salt;
    }

    // Prepara il blocco di input (padding con zeri se necessario)
    DES_cblock input = { 0 };
    memcpy(input, password.c_str(), std::min(password.size(), size_t(8)));

    // Esegue la cifratura DES
    DES_cblock output;
    DES_ecb_encrypt(&input, &output, &key_schedule, DES_ENCRYPT);

    // Converte l'output in formato esadecimale usando un buffer pre-allocato
    static thread_local char hex_output[17];  // 16 caratteri hex + null terminator
    for (int i = 0; i < 8; i++) {
        sprintf_s(hex_output + (i * 2), 3, "%02x", static_cast<unsigned char>(output[i]));
    }

    return std::string(hex_output, 16);
}