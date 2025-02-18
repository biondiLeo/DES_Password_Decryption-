#ifndef METRICS_HANDLER_H
#define METRICS_HANDLER_H

#include <string>
#include <vector>

/**
 * @brief Struttura per memorizzare le metriche di performance dei test
 *
 * Contiene tutte le metriche rilevanti per valutare le prestazioni
 * dell'algoritmo sia in modalità sequenziale che parallela.
 */
struct TestMetrics {
    int num_threads = 0;      ///< Numero di thread utilizzati
    int chunk_size = 0;       ///< Dimensione del chunk per la suddivisione del lavoro
    double min_time = 0.0;    ///< Tempo minimo di esecuzione
    double max_time = 0.0;    ///< Tempo massimo di esecuzione
    double mean_time = 0.0;   ///< Tempo medio di esecuzione
    double stddev = 0.0;      ///< Deviazione standard dei tempi
    double speedup = 0.0;     ///< Rapporto tra tempo sequenziale e parallelo
    double efficiency = 0.0;   ///< Efficienza della parallelizzazione (speedup/num_threads)
    int password_position = 0; ///< Posizione della password trovata
    int total_passwords = 0;   ///< Numero totale di password testate

    /**
     * @brief Costruttore che inizializza tutti i campi a zero
     */
    TestMetrics() {
        num_threads = 0;
        chunk_size = 0;
        min_time = 0.0;
        max_time = 0.0;
        mean_time = 0.0;
        stddev = 0.0;
        speedup = 0.0;
        efficiency = 0.0;
        password_position = 0;
        total_passwords = 0;
    }
};

/**
 * @brief Classe per la gestione e salvataggio delle metriche di performance
 */
class MetricsHandler {
public:
    /**
     * @brief Salva le metriche in un file CSV
     *
     * @param filename Nome del file di output
     * @param sequential_metrics Metriche dell'esecuzione sequenziale
     * @param parallel_metrics Metriche delle esecuzioni parallele
     */
    static void saveMetricsToCSV(const std::string& filename,
        const std::vector<TestMetrics>& sequential_metrics,
        const std::vector<std::vector<TestMetrics>>& parallel_metrics);

    /**
     * @brief Aggiunge nuove metriche al vettore delle metriche
     *
     * @param metrics_vector Vettore dove aggiungere le metriche
     * @param execution_times Tempi di esecuzione misurati
     * @param num_threads Numero di thread utilizzati
     * @param chunk_size Dimensione del chunk
     * @param sequential_time Tempo di esecuzione sequenziale di riferimento
     * @param password_position Posizione della password trovata
     * @param total_passwords Numero totale di password testate
     */
    static void appendTestMetrics(std::vector<TestMetrics>& metrics_vector,
        const std::vector<double>& execution_times,
        int num_threads,
        int chunk_size,
        double sequential_time,
        int password_position,
        int total_passwords);
};

#endif