# EchoConnect

Projekt ma docelowo umożliwiać łatwą komunikację pomiędzy komputerami nie wymagającą żadnych dodatkowych modułów (Bluetooth / WiFi), wykorzystując dźwięki o wysokiej częstotliwości. Stworzymy udokumentowany protokół transmisji wspierający korekcję błędów i sprawdzanie poprawności transmisji. Na jego podstawie zbudujemy mechanizm parowania komputerów (o podobnym mechanizmie działania jak w Bluetooth) umożliwiający przesyłanie plików lub tekstu między nimi. Całość zostanie opakowana w API udostępnione w formie biblioteki współdzielonej (.so / .dll). Oprócz tego stworzymy przykładowe aplikacje (CLI / QT) korzystające z tego API w celu zaprezentowania jego możliwości.


### Grupy użytkowników (use cases):
-   Osoby z wadliwym / ubogim sprzętem komputerowym
-   Hobbyści i ciekawscy
    
### Funkcjonalność:
-   Przesyłanie tekstu / plików
-   Korekcja błędów, weryfikacja poprawności
-   Parowanie komputerów
-   Autoryzacja tożsamości
-   API
-   Interfejs tekstowy (CLI)
-   Interfejs graficzny (QT)
    
### Używane technologie:
-   Język: C++
-   Obsługa dźwięku, GUI: QT
-   Testy: GoogleTest
-   Dokumentacja: Doxygen / QDoc
-   Linter: clang-tidy
-   Formatter: clang-format
    
### Narzędzia:
-   Repozytorium: Github
-   Issue tracker: Github Issues
-   Zarządzanie projektem: Github Projets
-   Continuous Integration: Github Actions


## Budowanie:
```
mkdir -p build
cd build
cmake ..
make
```
### Wymagane bilbioteki:
- QT
- GoogleTest
