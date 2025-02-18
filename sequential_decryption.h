#ifndef SEQUENTIAL_DECRYPTION_H
#define SEQUENTIAL_DECRYPTION_H

#include <string>
#include <vector>

/**
 * @brief Struttura per memorizzare i risultati dei test sequenziali
 *
 * Contiene le metriche di performance dell'algoritmo di decrittazione
 * sequenziale, inclusi tempi di esecuzione e posizione della password.
 */
struct SequentialResult {
    double mean_time;                   ///< Tempo medio di esecuzione
    std::vector<double> execution_times; ///< Array dei tempi di ogni esecuzione
    int position;                       ///< Posizione della password nella lista
};

/**
 * @brief Decrittazione sequenziale di una password
 *
 * Cerca di trovare la password originale provando ogni password nella lista
 * e confrontando il suo hash con quello fornito.
 *
 * @param encrypted_password Hash DES della password da trovare
 * @param salt Salt utilizzato per la generazione dell'hash
 * @param password_list Lista di password da testare
 * @return std::string Password decifrata o messaggio di errore
 */
std::string decryptPassword(const std::string& encrypted_password,
    const std::string& salt,
    const std::vector<std::string>& password_list);

/**
 * @brief Esegue test di performance sulla decrittazione sequenziale
 *
 * Esegue multiple prove di decrittazione con la password target posizionata
 * in diverse posizioni della lista, raccogliendo metriche di performance.
 *
 * @param password Password originale da cercare
 * @param salt Salt per la generazione dell'hash
 * @param password_list Lista di password da testare
 * @param executions Numero di esecuzioni del test
 * @return SequentialResult Risultati e statistiche dei test
 */
SequentialResult testSequential(const std::string& password,
    const std::string& salt,
    std::vector<std::string> password_list,
    int executions);

#endif // SEQUENTIAL_DECRYPTION_H