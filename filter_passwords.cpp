#include "filter_passwords.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> // Per controllare l'esistenza del file

using namespace std;

/**
 * @brief Verifica l'esistenza di un file nel filesystem
 *
 * Utilizza la libreria filesystem per verificare in modo sicuro
 * l'esistenza di un file nel percorso specificato.
 *
 * @param path Percorso del file da verificare
 * @return true se il file esiste, false altrimenti
 */
bool fileExists(const string& path) {
    return filesystem::exists(path);
}

/**
 * @brief Filtra le password secondo criteri specifici e le salva in un nuovo file
 *
 * La funzione legge un file contenente password, una per riga, e applica
 * i seguenti criteri di filtraggio:
 * - Lunghezza esattamente 8 caratteri
 * - Caratteri ammessi:
 *   - Lettere minuscole (a-z)
 *   - Lettere maiuscole (A-Z)
 *   - Numeri (0-9)
 *   - Caratteri speciali: punto (.) e slash (/)
 *
 * @param input_path Percorso del file contenente le password da filtrare
 * @param output_path Percorso dove salvare le password filtrate
 */
void filterPasswords(const string& input_path, const string& output_path) {
    // Verifica preventiva dell'esistenza del file di output
    // per evitare sovrascritture accidentali
    if (fileExists(output_path)) {
        cout << "Il file delle password filtrate esiste già: " << output_path << endl;
        return;
    }

    // Apertura del file di input con gestione errori
    ifstream input_file(input_path);
    if (!input_file) {
        cerr << "Errore: impossibile aprire il file di input: " << input_path << endl;
        return;
    }

    // Apertura del file di output con gestione errori
    ofstream output_file(output_path);
    if (!output_file) {
        cerr << "Errore: impossibile aprire il file di output: " << output_path << endl;
        return;
    }

    string password;
    vector<string> filtered_passwords;

    // Ciclo principale di elaborazione delle password
    while (getline(input_file, password)) {
        // Verifica della lunghezza esatta di 8 caratteri
        if (password.length() == 8) {
            bool valid = true;
            // Verifica che ogni carattere sia tra quelli ammessi
            for (char c : password) {
                if (!((c >= 'a' && c <= 'z') ||   // Lettere minuscole
                    (c >= 'A' && c <= 'Z') ||     // Lettere maiuscole
                    (c >= '0' && c <= '9') ||     // Numeri
                    c == '.' || c == '/')) {      // Caratteri speciali
                    valid = false;
                    break;
                }
            }
            // Se la password supera tutti i controlli, viene aggiunta
            if (valid) {
                filtered_passwords.push_back(password);
            }
        }
    }

    // Scrittura delle password filtrate nel file di output
    for (const auto& pwd : filtered_passwords) {
        output_file << pwd << endl;
    }

    // Stampa delle statistiche e chiusura dei file
    cout << "Numero di password filtrate trovate: " << filtered_passwords.size() << endl;
    cout << "Password filtrate salvate in: " << output_path << endl;

    // Chiusura sicura dei file
    input_file.close();
    output_file.close();
}