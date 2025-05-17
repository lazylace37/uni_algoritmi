# Progetto di Laboratorio di Algoritmi e Strutture Dati

## Prerequisiti

- Compilatore C
- Python
- Libreria Python `matplotlib`

## Compilazione

È necessario modificare il `Makefile` e rimuovere la voce `-mcpu=ampere1a` in
caso di compilazione su architetture non ARM.

Per compilare: `make`

## Esecuzione

Programma esecuzione algoritmi su un dato `n` e `m`:

```
./bench --help
Usage: ./bench <length> <max_value> <n_runs>
```

ad es.

`./bench 1000 50 10`

---

Programma esecuzione su più valori di `n` e `m`, e generazione di grafici comparativi:

```
python benchmark.py
```
