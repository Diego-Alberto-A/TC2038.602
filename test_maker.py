import random

#ingreso el tamaño de la lista
tamano = int(input())

lista = list(range(1,tamano+1))

random.shuffle(lista)
print(tamano)
print(*lista)