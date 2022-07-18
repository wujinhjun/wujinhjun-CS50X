# TODO
from cs50 import get_int

height = get_int("Height: ")
while 1 > height or height > 8:
    print("This input as invalid")
    height = get_int("Height: ")


for i in range(height):
    print(" " * (height - i - 1), end="")
    print("#" * (i + 1))