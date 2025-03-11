// Question 2: Fix the race conditions in the code. The code is supposed to add 5000 dollars
// to the wallet and parallelising it with threads. However, the final balance stored is sometimes not 5000 due
// to a race condition. The function is called a 1000 times to generate a race condition.
// https://stackoverflow.com/questions/34510/what-is-a-race-condition
// Think about how you can use a mutex to solve this

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

int total = 0; // not sure why this is declared and initialized but not used...

class Wallet
{
    int mMoney;
    std::mutex mtx;
public:
    Wallet() :mMoney(0) {}
    int getMoney() { return mMoney; }
    // the method addMoney() would cause the race conditions, as the 5 threads in main
    // would be trying to access mMoney at the same time.
    // "https://medium.com/@abhishekjainindore24/mutex-in-c-threads-part-1-45aeac3ab62d"
    // used ^ to find out more about race conditions and mutex in C
    // Learned about the different ways you could go about avoiding race conditions
    // using mutex and deduced that lock_guard would be the best way as we only 
    // need to lock a mutex in the scope of the for loop incrementing mMoney.
    void addMoney(int money) 
    {   

        for (int i = 0; i < money; ++i)
        {
            std::lock_guard<std::mutex> lock(mtx);
            mMoney++;
        }
    }
};


int fillWalletWithMoney()
{
    Wallet walletObject;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread(&Wallet::addMoney, &walletObject, 1000));
    }
    for (int i = 0; (unsigned)i < threads.size(); i++)
    {
        threads.at(i).join();
    }
    return walletObject.getMoney();
}
int main()
{
    int val = 0;
    for (int k = 0; k < 1000; k++)
    {
        if ((val = fillWalletWithMoney()) != 5000)
        {
            std::cout << "Error at count = " << k << " Money in Wallet = " << val << std::endl;
            return EXIT_FAILURE;
        }
    }
    return 0;
}