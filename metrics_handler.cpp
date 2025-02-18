#include "metrics_handler.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iomanip>

/**
 * @brief Implementazione del salvataggio delle metriche in formato CSV
 *
 * La funzione genera un file CSV con header appropriati e salva sia le metriche
 * sequenziali che quelle parallele, applicando controlli di validità sui dati.
 */
void MetricsHandler::saveMetricsToCSV(const std::string& filename,
    const std::vector<TestMetrics>& sequential_metrics,
    const std::vector<std::vector<TestMetrics>>& parallel_metrics) {
    std::ofstream file(filename);

    // Scrive l'header del CSV con tutte le colonne
    file << "test_type,num_threads,chunk_size,min_time,max_time,mean_time,stddev,speedup,efficiency,"
        << "password_position,total_passwords\n";

    // Scrive le metriche sequenziali
    for (const auto& metric : sequential_metrics) {
        // Verifica che i valori siano numeri finiti validi
        if (std::isfinite(metric.min_time) && std::isfinite(metric.max_time) &&
            std::isfinite(metric.mean_time) && std::isfinite(metric.stddev)) {
            file << "sequential," << metric.num_threads << ","
                << metric.chunk_size << ","
                << std::fixed << std::setprecision(6)
                << std::max(0.0, metric.min_time) << ","
                << std::max(0.0, metric.max_time) << ","
                << std::max(0.0, metric.mean_time) << ","
                << std::max(0.0, metric.stddev) << ","
                << std::max(0.0, metric.speedup) << ","
                << std::max(0.0, metric.efficiency) << ","
                << metric.password_position << ","
                << metric.total_passwords << "\n";
        }
    }

    // Scrive le metriche parallele (struttura nested per diversi test paralleli)
    for (const auto& thread_metrics : parallel_metrics) {
        for (const auto& metric : thread_metrics) {
            if (std::isfinite(metric.min_time) && std::isfinite(metric.max_time) &&
                std::isfinite(metric.mean_time) && std::isfinite(metric.stddev)) {
                file << "parallel," << metric.num_threads << ","
                    << metric.chunk_size << ","
                    << std::fixed << std::setprecision(6)
                    << std::max(0.0, metric.min_time) << ","
                    << std::max(0.0, metric.max_time) << ","
                    << std::max(0.0, metric.mean_time) << ","
                    << std::max(0.0, metric.stddev) << ","
                    << std::max(0.0, metric.speedup) << ","
                    << std::max(0.0, metric.efficiency) << ","
                    << metric.password_position << ","
                    << metric.total_passwords << "\n";
            }
        }
    }

    file.close();
}

/**
 * @brief Calcola e aggiunge nuove metriche al vettore delle metriche
 *
 * Calcola statistiche come media, deviazione standard, speedup ed efficienza
 * dai tempi di esecuzione forniti e aggiunge una nuova entry al vettore delle metriche.
 */
void MetricsHandler::appendTestMetrics(std::vector<TestMetrics>& metrics_vector,
    const std::vector<double>& execution_times,
    int num_threads,
    int chunk_size,
    double sequential_time,
    int password_position,
    int total_passwords) {
    if (execution_times.empty()) return;

    TestMetrics metrics;
    metrics.num_threads = num_threads;
    metrics.chunk_size = chunk_size;

    // Calcola le statistiche di base
    if (!execution_times.empty()) {
        metrics.min_time = *std::min_element(execution_times.begin(), execution_times.end());
        metrics.max_time = *std::max_element(execution_times.begin(), execution_times.end());
        metrics.mean_time = std::accumulate(execution_times.begin(), execution_times.end(), 0.0)
            / execution_times.size();

        // Calcola la deviazione standard
        double variance = 0.0;
        for (const auto& time : execution_times) {
            if (std::isfinite(time)) {
                variance += std::pow(time - metrics.mean_time, 2);
            }
        }
        metrics.stddev = std::sqrt(variance / execution_times.size());

        // Calcola speedup ed efficiency se i tempi sono validi
        if (sequential_time > 0 && metrics.mean_time > 0) {
            metrics.speedup = sequential_time / metrics.mean_time;
            metrics.efficiency = metrics.speedup / num_threads;
        }
    }

    metrics.password_position = password_position;
    metrics.total_passwords = total_passwords;

    metrics_vector.push_back(metrics);
}