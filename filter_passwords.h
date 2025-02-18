#ifndef FILTER_PASSWORDS_H
#define FILTER_PASSWORDS_H

#include <string>
using namespace std;

/**
 * @file filter_passwords.h
 * @brief Header file per le funzionalità di filtraggio delle password
 *
 * Questo file definisce l'interfaccia per la verifica e il filtraggio
 * delle password, fornendo funzioni per controllare l'esistenza dei file
 * e per applicare criteri di filtraggio alle password.
 */

 /**
  * @brief Verifica l'esistenza di un file nel sistema
  *
  * Controlla se un file esiste nel percorso specificato.
  * Utile per verificare la presenza dei file di input/output
  * prima di procedere con le operazioni di filtraggio.
  *
  * @param path Percorso del file da verificare
  * @return true se il file esiste, false altrimenti
  */
bool fileExists(const string& path);

/**
 * @brief Filtra le password secondo criteri specifici
 *
 * Legge le password da un file di input, applica i criteri
 * di filtraggio definiti e salva le password filtrate in
 * un file di output.
 *
 * @param input_path Percorso del file contenente le password da filtrare
 * @param output_path Percorso del file dove salvare le password filtrate
 */
void filterPasswords(const string& input_path, const string& output_path);

#endif // FILTER_PASSWORDS_H