#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <string>

/**
 * @brief Genera un hash DES di una password utilizzando un salt specifico
 *
 * @param password La password da cui generare l'hash
 * @param salt Il salt da utilizzare per l'hashing (deve essere di 8 byte)
 * @return std::string Hash in formato esadecimale di 16 caratteri
 *
 * @note L'implementazione utilizza l'algoritmo DES in modalità ECB
 * @warning DES è considerato obsoleto per la sicurezza moderna,
 *          si consiglia l'uso di algoritmi più robusti come AES
 */
std::string generateDESHash(const std::string& password, const std::string& salt);

#endif // HASH_UTILS_H