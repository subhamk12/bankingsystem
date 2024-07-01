#include "CubicProbing.h"
#include <vector>
using std::vector;
int numberOfAccounts = 0;

void quickSort(vector<int> &dataList, int start, int end)
{
    if (start >= end)
    {
        return;
    }
    int pivotValue = dataList[start];
    int pivotIndex = start;
    for (int i = start + 1; i <= end; i++)
    {
        if (dataList[i] <= pivotValue)
            pivotIndex++;
    }

    int temp = dataList[pivotIndex];
    dataList[pivotIndex] = dataList[start];
    dataList[start] = temp;

    int i = start;
    int j = end;

    while (i < pivotIndex && j > pivotIndex)
    {
        while (dataList[i] <= pivotValue)
        {
            i++;
        }

        while (dataList[j] > pivotValue)
        {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex)
        {

            int temp = dataList[i];
            dataList[i] = dataList[j];
            dataList[j] = temp;
            i++;
            j--;
        }
    }
    quickSort(dataList, start, pivotIndex - 1);

    quickSort(dataList, pivotIndex + 1, end);
}

void CubicProbing::createAccount(std::string id, int count)
{
    bankStorage1d = createHashTable();
    Account acc;
    acc.id = id;
    acc.balance = count;
    int hashValue = CubicProbing::hash(id);
    unsigned long long start = hashValue;
    unsigned long long i = 0;
    while (bankStorage1d[start].balance != -1 && i < hashSize)
    {
        i += 1;
        start += 3 * i * i + 3 * i - 1;
        start = start % hashSize;
    }
    if (i == hashSize)
    {
        start = hashValue + 1;
        while (start != hashValue)
        {
            if (bankStorage1d[start].balance == -1)
            {
                bankStorage1d[start] = acc;
                numberOfAccounts += 1;
                return;
            }
            start += 1;
        }
        return;
    }
    bankStorage1d[start] = acc;
    numberOfAccounts += 1;
    return;
}

std::vector<int> CubicProbing::getTopK(int k)
{
    std::vector<int> richList;
    for (int i = 0; i < hashSize; i++)
    {
        if (bankStorage1d[i].balance != -1)
        {
            richList.push_back(bankStorage1d[i].balance);
        }
    }
    int n = richList.size();
    quickSort(richList, 0, richList.size() - 1);

    std::vector<int> richestList;
    if (n >= k)
    {
        for (int i = 0; i < k; i++)
        {
            richestList.push_back(richList[n - i - 1]);
        }
        return richestList;
    }
    for (int i = 0; i < n; i++)
    {
        richestList.push_back(richList[n - i - 1]);
    }

    return richestList;
}

int CubicProbing::getBalance(std::string id)
{
    if (doesExist(id))
    {
        return bankStorage1d[accountIndex].balance;
    }
    return -1;
}

void CubicProbing::addTransaction(std::string id, int count)
{
    if (doesExist(id))
    {
        bankStorage1d[accountIndex].balance += count;
        return;
    }
    createAccount(id, count);
    return;
}

bool CubicProbing::doesExist(std::string id)
{
    int hashValue = hash(id);
    unsigned long long start = hashValue;
    unsigned long long i = 0;
    while ((bankStorage1d[start].balance != -1 || bankStorage1d[start].id == "Deleted") && i < hashSize)
    {
        if (bankStorage1d[start].id == id)
        {
            accountIndex = start;
            return true;
        }
        i += 1;
        start += 3 * i * i + 3 * i - 1;
        start = start % hashSize;
    }
    if (i == hashSize)
    {
        start = hashValue + 1;
        while (start != hashValue)
        {
            if (bankStorage1d[start].balance != -1 || bankStorage1d[start].id == "Deleted")
            {
                if (bankStorage1d[start].id == id)
                {
                    accountIndex = start;
                    return true;
                }
                start += 1;
            }
            accountIndex = -1;
            return false;
        }
        accountIndex = -1;
        return false;
    }
    accountIndex = -1;
    return false;
}

bool CubicProbing::deleteAccount(std::string id)
{
    if (doesExist(id))
    {
        bankStorage1d[accountIndex].id = "Deleted";
        bankStorage1d[accountIndex].balance = -1;
        numberOfAccounts -= 1;
        return true;
    }
    return false;
}
int CubicProbing::databaseSize()
{
    return numberOfAccounts;
}

int CubicProbing::hash(std::string id)
{
    unsigned long long val = 0;
    std::vector<int> N = {156137, 23719, 161983, 48517, 155162, 68321, 18061, 142253, 71853, 923351, 124563, 189253, 77783, 36351, 127973, 146783, 71433, 97851, 169963, 148763, 73343, 92441, 127963};
    int n = 13 * id[5] + 17 * id[7] + 19 * id[9] + 29 * id[13] + 23 * id[17];
    n = n % 7;
    for (int i = 0; i < 11; i++)
    {
        val += N[i] * id[N[n + i] % id.size()];
    }
    return val % hashSize;
}
