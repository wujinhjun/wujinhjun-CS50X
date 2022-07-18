#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void mergeSort(int left, int right);
void merge(int left, int mid, int right);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if(strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    string winnerNames[candidate_count];
    mergeSort(0, candidate_count - 1);
    winnerNames[0] = candidates[candidate_count - 1].name;
    int j = 1;
    for (int i = candidate_count - 2; i >= 0 ; i--)
    {
        if (candidates[i].votes >= candidates[candidate_count - 1].votes)
        {
            winnerNames[j] = candidates[i].name;
            j++;
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < j; i++)
    {
        printf("%s\n", winnerNames[i]);
    }
    return;
}

// mergeSort
void mergeSort(int left, int right)
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(left, mid, right);
    }
}

// merge
void merge(int left, int mid, int right)
{
    candidate temp[candidate_count];
    // candidates
    for (int i = left; i <= right; i++)
    {
        temp[i] = candidates[i];
    }

    int i = left;
    int j = mid + 1;
    int k = left;
    while (i < mid + 1 && j < right + 1)
    {
        temp[k++] = (candidates[i].votes <= candidates[j].votes) ? candidates[i++] : candidates[j++];
    }
    while (j < right + 1)
    {
        temp[k++] = candidates[j++];
    }
    while (i < mid + 1)
    {
        temp[k++] = candidates[i++];
    }

    for (i = left; i <= right; i++)
    {
        candidates[i] = temp[i];
    }
}