#ifndef PARALLEL_DECRYPTION_H
#define PARALLEL_DECRYPTION_H

#include <string>
#include <vector>
#include <omp.h> 

/**
 * @brief Struttura per memorizzare i risultati dei test paralleli
 *
 * Contiene i tempi di esecuzione e altre metriche rilevanti per
 * l'analisi delle prestazioni della decrittazione parallela.
 */
struct ParallelResult {
    std::vector<double> mean_times;      ///< Tempi medi per diverse configurazioni
    std::vector<double> execution_times;  ///< Tutti i tempi di esecuzione
    int position;                        ///< Posizione della password trovata
    std::vector<int> chunk_sizes;        ///< Dimensioni dei chunk testati
};

/**
 * @brief Abilita la cancellazione dei task OpenMP
 *
 * Permette di interrompere l'esecuzione degli altri thread
 * quando la password viene trovata.
 */
void enableOpenMPCancellation();

/**
 * @brief Implementa la decrittazione parallela usando OpenMP
 *
 * @param encrypted_password Password cifrata da decrittare
 * @param salt Salt utilizzato per la cifratura
 * @param password_list Lista di password da testare
 * @param num_threads Numero di thread da utilizzare
 * @param chunk_size Dimensione del chunk per la suddivisione del lavoro
 * @return std::string Password decifrata o messaggio di errore
 */
std::string openMPDecryption(const std::string& encrypted_password, const std::string& salt,
    const std::vector<std::string>& password_list, int num_threads, int chunk_size);

/**
 * @brief Esegue test di performance sulla decrittazione parallela
 *
 * Effettua multiple esecuzioni con diverse configurazioni di thread e chunk size,
 * raccogliendo metriche dettagliate sulle prestazioni.
 *
 * @param password Password originale per la verifica
 * @param salt Salt per la generazione dell'hash
 * @param password_list Lista di password da testare
 * @param thread_counts Array con i numeri di thread da testare
 * @param executions Numero di esecuzioni per ogni configurazione
 * @return std::vector<ParallelResult> Risultati dei test
 */
std::vector<ParallelResult> testParallel(const std::string& password, const std::string& salt,
    std::vector<std::string> password_list, const std::vector<int>& thread_counts, int executions);

#endif // PARALLEL_DECRYPTION_H