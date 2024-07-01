#ifndef QUADRATICPROBING_H
#define QUADRATICPROBING_H

#include "BaseClass.h"
#include <iostream>

class QuadraticProbing : public BaseClass
{
public:
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;

private:
    int hashSize = 190063;
    int accountIndex = 0;
    std::vector<Account> createHashTable()
    {
        if (bankStorage1d.size() > 0)
        {
            return bankStorage1d;
        }
        Account acc;
        acc.id = "";
        acc.balance = -1;
        bankStorage1d.assign(hashSize, acc);
        return bankStorage1d;
    }
    // Other data members and functions specific to Quadratic Probing
};

#endif // QUADRATICPROBING_H
