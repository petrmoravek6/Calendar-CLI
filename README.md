# Kalendář
CLI Aplikace pro správu kalendáře s možností vytváření událostí a jejich hledání.

## Funkcionality
- Denní, týdenní a měsíční zobrazení kalendáře
- Vytváření, správa a zobrazení jednotlivých událostí (čas, místo, datum, účastníci,..)
- Možnost nastavit události opakování (jednou za týden, 14 dní, ...)
- Algoritmus umožňující přesouvání událostí na nejbližší možný volný termín
- Hledání události podle názvu a/nebo místa konání
- Ukládání a načítání kalendáře

## Spuštění
### Příkazy
- **make (make compile)** - vytvoří potřebné object files a spustitelný soubor
- **make run** - make + spustí aplikaci
- **make clean** - smaže dočasné a nepotřebné soubory
### Soubory
- **doc/index.html** - dokumentace
- **examples/load_me.json** - pro vyzkoušení funkcionalit aplikace doporučuji během výzvy po spuštění aplikace k načtení kalendáře ze souboru zadat cestu k tomuto souboru

**upozornění** - aplikace umí pouze ASCII znaky (bez diakritiky), nejnižší možné datum je 1.1.1970 a maximální datum je 31.12.2037 
