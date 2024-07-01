#include "Chaining.h"

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

void Chaining::createAccount(std::string id, int count)
{
    Account acc;
    acc.id = id;
    acc.balance = count;

    int hashValue = hash(id);

    if (Chaining::bankStorage2d.size() == 0)
    {
        Chaining::bankStorage2d.assign(hashSize, bankStorage1d);
        Chaining::bankStorage2d[hashValue].push_back(acc);
        numberOfAccounts++;
    }
    else
    {
        Chaining::bankStorage2d[hashValue].push_back(acc);

        numberOfAccounts++;
    }
}

std::vector<int> Chaining::getTopK(int k)
{
    std::vector<int> richList;
    for (int i = 0; i < hashSize; i++)
    {
        for (int j = 0; j < bankStorage2d[i].size(); j++)
        {
            richList.push_back(bankStorage2d[i][j].balance);
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

int Chaining::getBalance(std::string id)
{
    int hashValue = hash(id);
    for (int i = 0; i < bankStorage2d[hashValue].size(); i++)
    {
        if (bankStorage2d[hashValue][i].id == id)
        {
            return bankStorage2d[hashValue][i].balance;
        }
    }
    return -1;
}

void Chaining::addTransaction(std::string id, int count)
{
    int hashValue = hash(id);
    for (int i = 0; i < bankStorage2d[hashValue].size(); i++)
    {
        if (bankStorage2d[hashValue][i].id == id)
        {
            bankStorage2d[hashValue][i].balance += count;
            return;
        }
    }
    createAccount(id, count);
    return;
}

bool Chaining::doesExist(std::string id)
{
    int hashValue = hash(id);
    for (int i = 0; i < bankStorage2d[hashValue].size(); i++)
    {
        if (bankStorage2d[hashValue][i].id == id)
        {
            return true;
        }
    }
    return false;
}

bool Chaining::deleteAccount(std::string id)
{
    int hashValue = hash(id);

    for (int i = 0; i < bankStorage2d[hashValue].size(); i++)
    {
        if (bankStorage2d[hashValue][i].id == id)
        {
            bankStorage2d[hashValue].erase(bankStorage2d[hashValue].begin() + i);
            numberOfAccounts--;
            return true;
        }
    }
    return false;
}
int Chaining::databaseSize()
{

    return numberOfAccounts;
}

int Chaining::hash(std::string id)
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