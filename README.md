# TKOM/ZPR - Projekt
*Bartosz Nowak, 325201*

## Opis Projektu

Tematem projektu jest implementacja kompilatora własnego języka programowania

## Sposób zbudowania i uruchomienia

Program można zbudować za pomocą skryptu `build.sh`. Wymagane będą:
- narzędzie `CMake`
- biblioteka `Boost`

Zbudowany program możemy uruchomić za pomocą:

```sh
./tkom-lexer [filename] [-V]
```
Powyższa wersja programu jest implementacją leksera i parsera.

Gdzie flaga `-V (--verbose)` wypisze na ekran znalezione tokeny.

Dostępna jest również wersja programu implementująca jedynie lekser. Może być ona uruchomiona za pomocą:

```sh
./tkom-lexer [filename] [-V]
```

## Testowanie

W katalogu `tests` zawarte są testy jednostkowe dla leksera używające biblioteki Google Test. Testy mogą zostać uruchomione za pomocą polecenia `ctest` w katalogu `build`.
