#include "filter_passwords.h"
#include "sequential_decryption.h"
#include "parallel_decryption.h"
#include "metrics_handler.h"
#include <omp.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <stdlib.h>

/**
 * @brief Carica le password filtrate da file
 *
 * Legge le password precedentemente filtrate da un file di testo,
 * dove ogni riga contiene una password.
 *
 * @param file_path Percorso del file contenente le password filtrate
 * @return std::vector<std::string> Vettore contenente tutte le password
 */
std::vector<std::string> loadFilteredPasswords(const std::string& file_path) {
    std::vector<std::string> passwords;
    std::ifstream file(file_path);
    std::string password;

    while (getline(file, password)) {
        passwords.push_back(password);
    }

    return passwords;
}

/**
 * @brief Ottimizza l'ambiente per l'esecuzione parallela
 *
 * Esegue un warm-up del sistema parallelo allocando memoria
 * e inizializzando i thread OpenMP.
 */
void optimizeForParallel() {
#pragma omp parallel
    {
#pragma omp single
        {
            int num_threads = omp_get_num_threads();
            std::vector<char> dummy(1024 * 1024);  // Allocazione dummy per warm-up
        }
    }
}

/**
 * @brief Funzione principale del programma
 *
 * Implementa il flusso completo del sistema di testing:
 * 1. Filtraggio delle password
 * 2. Test sequenziale
 * 3. Test paralleli con diverse configurazioni
 * 4. Raccolta e salvataggio delle metriche
 */
int main() {
#ifdef _OPENMP
    std::cout << "OpenMP Version: " << _OPENMP << std::endl;

    // Configurazione dei parametri di input
    std::string input_file = "data/rockyou.txt";        ///< File sorgente delle password
    std::string filtered_file = "data/filtered_passwords.txt";  ///< File delle password filtrate
    std::string metrics_file = "benchmark_metrics.csv";  ///< File output delle metriche
    std::string target_password = "ParaComp";           ///< Password da cercare
    std::string salt = "Leonardo8";                     ///< Salt per la generazione hash

    // Strutture dati per le metriche
    std::vector<TestMetrics> sequential_metrics;
    std::vector<std::vector<TestMetrics>> parallel_metrics;

    // FASE 1: Filtraggio password
    std::cout << "Filtraggio delle password in corso..." << std::endl;
    filterPasswords(input_file, filtered_file);
    std::cout << "Password filtrate salvate in: " << filtered_file << std::endl;

    // FASE 2: Caricamento password filtrate
    std::cout << "Caricamento della lista completa delle password..." << std::endl;
    std::vector<std::string> password_list = loadFilteredPasswords(filtered_file);
    std::cout << "Numero totale di password caricate: " << password_list.size() << std::endl;

    // Ottimizzazione ambiente parallelo
    optimizeForParallel();

    // Configurazione dei test
    int executions = 10;  ///< Numero di esecuzioni per ogni configurazione

    // FASE 3: Test sequenziale
    std::cout << "\nInizio test sequenziale con " << executions << " esecuzioni..." << std::endl;
    auto seq_result = testSequential(target_password, salt, password_list, executions);

    // Raccolta metriche sequenziali
    MetricsHandler::appendTestMetrics(sequential_metrics,
        seq_result.execution_times,
        1,      // Un solo thread per l'esecuzione sequenziale
        0,      // Chunk size non applicabile per sequenziale
        seq_result.mean_time,
        seq_result.position,
        password_list.size());

    double mean_sequential = seq_result.mean_time;

    // Configurazione test paralleli
    int max_threads = omp_get_max_threads();
    std::cout << "Numero massimo di thread disponibili: " << max_threads << std::endl;

    // Definizione delle configurazioni di thread da testare
    std::vector<int> thread_counts = { 2, 4, 6, 8 };

    // FASE 4: Test paralleli
    auto parallel_results = testParallel(target_password, salt,
        password_list,
        thread_counts,
        executions);

    // Raccolta metriche per ogni configurazione parallela
    for (size_t i = 0; i < thread_counts.size(); ++i) {
        for (size_t j = 0; j < parallel_results[i].chunk_sizes.size(); ++j) {
            std::vector<TestMetrics> thread_metrics;
            MetricsHandler::appendTestMetrics(thread_metrics,
                parallel_results[i].execution_times,
                thread_counts[i],
                parallel_results[i].chunk_sizes[j],
                mean_sequential,
                parallel_results[i].position,
                password_list.size());

            parallel_metrics.push_back(thread_metrics);

            // Output dello speedup
            std::cout << "Speedup con " << thread_counts[i] << " thread: "
                << thread_metrics[0].speedup << std::endl;
        }
    }

    // FASE 5: Salvataggio metriche
    MetricsHandler::saveMetricsToCSV(metrics_file, sequential_metrics, parallel_metrics);
    std::cout << "\nMetriche salvate in: " << metrics_file << std::endl;

    return 0;
#else
    std::cout << "OpenMP non è supportato" << std::endl;
#endif
}