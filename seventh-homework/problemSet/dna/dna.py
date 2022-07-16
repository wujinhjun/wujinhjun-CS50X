import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    dataFile = sys.argv[1]
    dnaList = []
    subsequenceList = []
    with open(dataFile, "r") as file:
        reader = csv.DictReader(file)
        dnaList = list(reader)

    with open(dataFile, "r") as file:
        # -----------------------
        reader = csv.reader(file)
        for row in reader:
            subsequenceList = row
            subsequenceList.pop(0)
            # print(header)
            break
        # this part(between the line) from https://gist.github.com/ndiecodes/c475fc438580d0421edd9d3ee75b95d5
        # ----------------------

    # print(subsequenceList)
    # print(dnaList)

    # TODO: Read DNA sequence file into a variable
    seqFile = sys.argv[2]
    with open(seqFile, "r") as file:
        sequenceContent = file.read()
    # print(sequenceContent)

    # TODO: Find longest match of each STR in DNA sequence
    matchDict = {}
    for subs in subsequenceList:
        matchDict[subs] = longest_match(sequenceContent, subs)
        # match_list.append(longest_match(sequenceContent, subs))
    # print(match_Dict)

    # TODO: Check database for matching profiles
    result = check(dnaList, subsequenceList, matchDict)
    print(result)

    return


def check(dnaList, subsequenceList, matchDict):
    for person in dnaList:
        # print(person)
        result = "No match"
        count = 0
        for item in subsequenceList:
            # print(f"{person[item]} : {matchDict[item]}")
            # print(int(person[item]) == int(match_Dict[item]))
            if int(person[item]) != int(matchDict[item]):
                # print("yes")
                # continue
                break
            # else:
                # break
            count += 1
            if count >= len(matchDict):
                return person['name']
    return result


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
