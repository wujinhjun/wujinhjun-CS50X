# TODO
from cs50 import get_int


def main():
    card_number = get_int("Number: ")
    while card_number < 0:
        card_number = get_int("Number: ")
    # print(card_number)

    number_list = list(map(int, str(card_number)))
    # print(number_list)
    if judgeValid(number_list):
        judgeCompany(number_list)
    else:
        print("INVALID")


def judgeValid(number):
    sum = 0
    number_list = reversed(number)
    for i, key in enumerate(number_list):
        if i % 2 == 0:
            sum += int(key)
        else:
            for j in str(int(key) * 2):
                sum += int(j)
    if (sum % 10 == 0):
        return True
    else:
        return False


def judgeCompany(number_list):
    # print(number_list)
    title = str(number_list[0]) + str(number_list[1])
    length = len(number_list)
    print(title[0])
    type = ""
    if (length == 13 or length == 16) and (int(title[0]) == 4):
        type = "VISA"
    elif (length == 15 and (int(title) == 34 or int(title) == 37)):
        type = "AMEX"
    elif (length == 16 and int(title) >= 51 and int(title) <= 55):
        type = "MASTERCARD"
    print(type)


if __name__ == "__main__":
    main()
