# Tema 1 - Sisteme de Programe pentru Retele de Calculatoare - RPC

## Pasii de implementare
* In client.cpp citesc comenzile (SEARCH / APPEND / exit - ca sa se inchida
clientul). In server.cpp am o functie (get_server_response_1_svc), care primeste
comanda data de user, se populeaza, daca e gol, hashtable-ul cu perechi de tipul
cuvant - numar de aparitii (am o functie care imi sterge delimitatorii din
cuvinte). Daca user-ul trimite o comanda invalida, se intra intr-un while, din
care se iese pana cand se primeste un raspuns pozitiv din partea serverului.
* In client.cpp, in functie de ce comanda data de utilizator (asta dupa ce s-a
primit un raspuns pozitiv din partea server-ului), avem in felul urmator:
    * cand avem SEARCH, ne folosim de hashtable, pentru a nu mai citi din fisier,
pur si simplu se itereaza prin hashmap si numaram numarul de aparitii al unui
cuvant dat sau numarul de cuvinte are au o lungime data de user.
    * cand avem APPEND, deschidem fisierul si adaugam cuvantul dat in fisier, apoi
punem cuvantul in dictionar, daca nu exista, cu valoarea 1, sau, daca exista deja, ii
incrementam valoarea cu 1. Daca scrierea in fisier sau deschiderea / inchiderea
fisierului au esuat, serverul trimite raspuns de adaugare esuata (in caz contrar
se trimite un mesaj pozitiv + numarul de aparitii in fisier).