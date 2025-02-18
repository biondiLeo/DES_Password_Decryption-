#include "parallel_decryption.h"
#include "hash_utils.h"
#include <omp.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <atomic>
#include <immintrin.h>
#include <windows.h>
using namespace std;

/**
 * @brief Implementazione della decrittazione parallela con OpenMP
 *
 * Utilizza OpenMP per distribuire il carico di lavoro tra i thread disponibili,
 * con supporto per la terminazione anticipata quando la password viene trovata.
 */
std::string openMPDecryption(const std::string& encrypted_password, const std::string& salt,
    const std::vector<std::string>& password_list, int num_threads, int chunk_size) {
    std::string decrypted_password = "Password Not Found!";
    bool found = false;

    // Parallelizza la ricerca della password
#pragma omp parallel num_threads(num_threads) shared(found, decrypted_password)
    {
        std::string local_result;
#pragma omp for schedule(dynamic, chunk_size)
        for (int i = 0; i < static_cast<int>(password_list.size()); ++i) {
            if (!found) {  // Check per terminazione anticipata
                std::string hash = generateDESHash(password_list[i], salt);

                if (hash == encrypted_password) {
#pragma omp critical  // Sezione critica per aggiornare il risultato
                    {
                        if (!found) {
                            found = true;
                            decrypted_password = password_list[i];
                        }
                    }
                }
            }
        }
    }

    return decrypted_password;
}

/**
 * @brief Esegue test approfonditi sulla decrittazione parallela
 *
 * Implementa una suite di test completa che:
 * 1. Configura l'ambiente di test (priorità processo, affinità CPU)
 * 2. Esegue warm-up del sistema
 * 3. Testa diverse configurazioni di thread e chunk size
 * 4. Raccoglie e analizza metriche dettagliate
 */
std::vector<ParallelResult> testParallel(const std::string& password, const std::string& salt,
    std::vector<std::string> password_list, const std::vector<int>& thread_counts, int executions) {
    std::vector<ParallelResult> all_results;

    // Configura diverse dimensioni di chunk da testare
    std::vector<int> chunk_sizes = { 500, 1000, 2000, 4000 };

    // Ottimizza la priorità del processo
    HANDLE process = GetCurrentProcess();
    SetPriorityClass(process, HIGH_PRIORITY_CLASS);

    // Esegue warm-up per stabilizzare le prestazioni
#pragma omp parallel
    {
        volatile std::string hash = generateDESHash("warmup", salt);
    }

    // Test con diversi numeri di thread
    for (int num_threads : thread_counts) {
        ParallelResult thread_result;
        std::cout << "\n=== Test Parallelo con " << num_threads << " thread ===\n";

        // Test con diverse dimensioni di chunk
        for (int chunk_size : chunk_sizes) {
            std::cout << "\n--- Testing with chunk size: " << chunk_size << " ---\n";

            // Configura l'affinità dei thread
            DWORD_PTR mask = (1ULL << num_threads) - 1;
            SetProcessAffinityMask(process, mask);

            std::vector<double> execution_times;
            std::vector<size_t> positions;
            const size_t list_size = password_list.size();

            // Distribuisce uniformemente la password target
            for (int i = 0; i < executions; ++i) {
                size_t pos = (list_size / executions) * i + (list_size / executions / 2);
                positions.push_back(pos);
            }

            // Configura OpenMP
            omp_set_dynamic(0);
            omp_set_num_threads(num_threads);

            // Esegue i test multipli
            for (int i = 0; i < executions; ++i) {
                // Inserisce la password nella posizione calcolata
                if (i < executions - 1) {
                    password_list.insert(password_list.begin() + positions[i], password);
                }

                std::string encrypted_password = generateDESHash(password, salt);

                // Pulisce la cache per test più accurati
                _mm_mfence();
                std::vector<char> cache_clear(1024 * 1024, 1);
                for (volatile char& c : cache_clear) {
                    c = 2;
                }

                // Misura il tempo di esecuzione
                auto start = std::chrono::high_resolution_clock::now();
                std::string result = openMPDecryption(encrypted_password, salt, password_list,
                    num_threads, chunk_size);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> elapsed = end - start;
                execution_times.push_back(elapsed.count());

                // Ripristina la lista password
                if (i < executions - 1) {
                    password_list.erase(password_list.begin() + positions[i]);
                }

                // Output dei risultati
                cout << "Esecuzione " << i + 1 << " (chunk size " << chunk_size << "): "
                    << "Tempo: " << elapsed.count() << " secondi. "
                    << "Posizione: " << positions[i] << endl;
            }

            // Calcola le statistiche
            double min_time = *std::min_element(execution_times.begin(), execution_times.end());
            double max_time = *std::max_element(execution_times.begin(), execution_times.end());
            double mean_time = std::accumulate(execution_times.begin(), execution_times.end(), 0.0)
                / execution_times.size();

            // Calcola la deviazione standard
            double variance = 0.0;
            for (const auto& time : execution_times) {
                variance += std::pow(time - mean_time, 2);
            }
            double stddev = std::sqrt(variance / execution_times.size());

            // Output delle statistiche
            cout << "\nStatistiche Test Parallelo (" << num_threads << " thread, "
                << "chunk size " << chunk_size << "):" << endl;
            cout << "- Tempo minimo: " << min_time << " secondi" << endl;
            cout << "- Tempo massimo: " << max_time << " secondi" << endl;
            cout << "- Tempo medio: " << mean_time << " secondi" << endl;
            cout << "- Deviazione standard: " << stddev << " secondi" << endl;

            // Salva i risultati
            thread_result.mean_times.push_back(mean_time);
            thread_result.execution_times = execution_times;
            thread_result.position = positions[positions.size() - 1];
            thread_result.chunk_sizes.push_back(chunk_size);
        }

        all_results.push_back(thread_result);
    }

    return all_results;
}