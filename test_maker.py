import random

tamano = int(input())

lista = list(range(tamano))

random.shuffle(lista)

print(*lista)