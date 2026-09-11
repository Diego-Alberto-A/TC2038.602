import random

#ingreso el tamaño de la lista
tamano = int(input())

lista = list(range(tamano))

random.shuffle(lista)
print(tamano)
print(*lista)