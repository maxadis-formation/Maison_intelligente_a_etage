/*
 * ============================================================================
 * PROJET : Centrale Domotique d'Éclairage avec Fonction Minuterie Temporisée
 * MAQUETTE : Maison intelligente à étage (éclairage intérieur sur deux niveaux)
 * CENTRE : Centre de Formation Professionnelle MAX_ADIS (Édition 2026)
 * RÉALISÉ PAR :
 *   - HOUNMENOU Martial
 *   - AGBANRIN Ishola
 *   - HOUANSOU Régis A.
 * ============================================================================
 */

#include <SoftwareSerial.h> // Bibliothèque de communication série pour le module Bluetooth

// Déclaration des broches et de la liaison Bluetooth (RX = 10, TX = 11)
SoftwareSerial mySerial(10, 11); // RX, TX vers HC-05 / HC-06
const int relay1 = 2;            // Relais 1 : Éclairage Niveau 1 (Rez-de-chaussée)
const int relay2 = 3;            // Relais 2 : Éclairage Niveau 2 (Étage supérieur)

void setup() {
  // Configuration des broches des relais en sortie
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // État initial : les deux lampes sont éteintes (Relais actifs à l'état BAS / LOW)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  // Initialisation du moniteur série matériel (USB)
  Serial.begin(9600);
  while (!Serial) {
    ; // Attente de connexion du port série
  }
  Serial.println("DEMARRAGE DU SYSTEME!");

  // Initialisation de la liaison Bluetooth à 9600 bauds
  mySerial.begin(9600);
  mySerial.println("INITIALISATION !");
}

void loop() {
  // Lecture des commandes reçues depuis l'application mobile via Bluetooth
  if (mySerial.available()) {
    char recu1 = mySerial.read();
    Serial.write(recu1); // Affichage sur le moniteur série pour le débogage

    // --- COMMANDE LAMPE 1 (Rez-de-chaussée) ---
    if (recu1 == 'A') { // ON 1
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      Serial.println(" -> Commande A : Lampe 1 (RDC) ALLUMEE");
    }

    if (recu1 == 'B') { // OFF 1
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      Serial.println(" -> Commande B : Lampe 1 (RDC) ETEINTE");
    }

    // --- COMMANDE LAMPE 2 (Étage supérieur) ---
    if (recu1 == 'C') { // ON 2
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      Serial.println(" -> Commande C : Lampe 2 (Etage) ALLUMEE");
    }

    if (recu1 == 'D') { // OFF 2
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      Serial.println(" -> Commande D : Lampe 2 (Etage) ETEINTE");
    }

    // --- FONCTION MINUTERIE TEMPORISÉE 1 (10 secondes - Niveau 1) ---
    if (recu1 == 'T') { // TIMER 10 secondes sur Lampe 1
      Serial.println(" -> Commande T : Minuterie 1 activee (10 secondes)");
      digitalWrite(relay1, LOW);   // Allume le relais 1
      digitalWrite(relay2, HIGH);
      delay(10000);                // Attend 10 secondes (10 000 ms)
      digitalWrite(relay1, HIGH);  // Éteint automatiquement les relais
      digitalWrite(relay2, HIGH);
      Serial.println(" -> Fin Minuterie 1 : Extinction automatique");
    }

    // --- FONCTION MINUTERIE TEMPORISÉE 2 (10 secondes - Niveau 2) ---
    if (recu1 == 'U') { // TIMER 10 secondes sur Lampe 2
      Serial.println(" -> Commande U : Minuterie 2 activee (10 secondes)");
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);   // Allume le relais 2
      delay(10000);                // Attend 10 secondes (10 000 ms)
      digitalWrite(relay1, HIGH);  // Éteint automatiquement les relais
      digitalWrite(relay2, HIGH);
      Serial.println(" -> Fin Minuterie 2 : Extinction automatique");
    }
  }

  // Pont série optionnel du PC vers le module Bluetooth
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
