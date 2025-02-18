#include "sequential_decryption.h"
#include "hash_utils.h"
#include <Windows.h>
#include <bcrypt.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <cmath> 

using namespace std;

/**
 * @brief Implementa l'algoritmo di decrittazione sequenziale
 *
 * Cerca la password confrontando l'hash di ogni password nella lista
 * con l'hash fornito, fino a trovare una corrispondenza.
 */
std::string decryptPassword(const std::string& encrypted_password,
    const std::string& salt, const vector<std::string>& password_list) {
    // Itera attraverso tutte le password nella lista
    for (const auto& word : password_list) {
        std::string hash = generateDESHash(word, salt);
        if (hash == encrypted_password) {
            return word; // Password trovata
        }
    }
    return "Password Not Found!";
}

/**
 * @brief Esegue test approfonditi sulla decrittazione sequenziale
 *
 * Implementa una suite di test che:
 * 1. Distribuisce la password target in diverse posizioni
 * 2. Misura i tempi di esecuzione
 * 3. Calcola statistiche dettagliate sulle performance
 */
SequentialResult testSequential(const string& password, const string& salt,
    vector<string> password_list, int executions) {
    vector<double> execution_times;
    vector<int> positions;

    // Calcola le posizioni di test distribuite uniformemente
    int step = password_list.size() / executions;
    for (int i = 0; i < executions - 1; ++i) {
        positions.push_back(i * step);
    }
    // Aggiunge un test con password non presente (caso peggiore)
    positions.push_back(password_list.size());

    cout << "\n=== Test Sequenziale ===\n";

    // Esegue i test per ogni posizione
    for (int i = 0; i < executions; ++i) {
        // Inserisce la password nella posizione di test
        if (i < executions - 1) {
            password_list.insert(password_list.begin() + positions[i], password);
        }

        // Prepara e avvia il test
        string encrypted_password = generateDESHash(password, salt);
        auto start = chrono::high_resolution_clock::now();
        string result = decryptPassword(encrypted_password, salt, password_list);
        auto end = chrono::high_resolution_clock::now();

        // Calcola e registra il tempo di esecuzione
        chrono::duration<double> elapsed = end - start;
        execution_times.push_back(elapsed.count());

        // Output dei risultati del test
        if (result == password) {
            cout << "Esecuzione " << i + 1 << ": Password trovata in " << elapsed.count()
                << " secondi. Posizione: " << positions[i] << endl;
        }
        else {
            cout << "Esecuzione " << i + 1 << ": Password non trovata. Tempo: "
                << elapsed.count() << " secondi." << endl;
        }

        // Ripristina la lista password per il prossimo test
        if (i < executions - 1) {
            password_list.erase(password_list.begin() + positions[i]);
        }
    }

    // Calcola le statistiche di performance
    double min_time = *min_element(execution_times.begin(), execution_times.end());
    double max_time = *max_element(execution_times.begin(), execution_times.end());
    double mean_time = accumulate(execution_times.begin(), execution_times.end(), 0.0)
        / execution_times.size();

    // Calcola la deviazione standard
    double variance = 0.0;
    for (const auto& time : execution_times) {
        variance += (time - mean_time) * (time - mean_time);
    }
    double stddev = sqrt(variance / execution_times.size());

    // Output delle statistiche complete
    cout << "\nStatistiche Test Sequenziale:" << endl;
    cout << "- Tempo minimo: " << min_time << " secondi" << endl;
    cout << "- Tempo massimo: " << max_time << " secondi" << endl;
    cout << "- Tempo medio: " << mean_time << " secondi" << endl;
    cout << "- Deviazione standard: " << stddev << " secondi" << endl;
    cout << "==============================\n" << endl;

    // Prepara e restituisce i risultati
    SequentialResult result;
    result.mean_time = mean_time;
    result.execution_times = execution_times;
    result.position = positions[positions.size() - 1];
    return result;
}