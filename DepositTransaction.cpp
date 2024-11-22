#include "DepositTransaction.hpp"
#include <iostream>
#include "CashManager.hpp"
#include "Exceptions.hpp"
#include "SystemStatus.hpp"
#include "InputHandler.hpp"
#include "Bank.hpp"

DepositTransaction::DepositTransaction(const std::string& transactionID, int amount, const std::shared_ptr<Account>& account,
                                       DepositType depositType, const std::string& cardNumber)
    : ITransaction(transactionID, amount, cardNumber), account(account), depositType(depositType), fee(0) {}

bool DepositTransaction::execute() {
    auto systemStatus = SystemStatus::getInstance();
    Bank* globalBank = systemStatus->getBank();

    // 수수료 계산
    if (account->getBankName() == globalBank->getBankName()) {
        fee = 1000; // 기본 은행
    } else {
        fee = 2000; // 타 은행
    }

    int totalInserted = 0;
    int totalBills = 0;
    std::map<Denomination, int> insertedCash;

    if (depositType == DepositType::CASH) {
        std::cout << "Please insert cash by specifying the number of bills for each denomination." << std::endl;
        for (const auto& denomPair : DENOMINATION_VALUES) {
            int count = 0;
            while (true) {
                auto countVariant = InputHandler::getInput("Number of KRW " + std::to_string(denomPair.second) + " bills: ", InputType::INT);
                try {
                    count = std::get<int>(countVariant);
                    if (count < 0) {
                        std::cout << "Invalid count. Please enter a non-negative number." << std::endl;
                        continue;
                    }
                    break;
                } catch (const std::bad_variant_access&) {
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                }
            }
            insertedCash[denomPair.first] = count;
            totalInserted += denomPair.second * count;
            totalBills += count;
        }

        if (totalInserted < fee) {
            std::cout << "Inserted cash is insufficient to cover the fee of KRW " << fee << "." << std::endl;
            return false;
        }

        if (totalBills > MAX_CASH_DEPOSIT) {
            std::cout << "Exceeded maximum number of cash bills allowed per transaction." << std::endl;
            return false;
        }

        // 수수료 차감 후 실제 입금 금액 계산
        int depositAmount = totalInserted - fee;

        // 현금 수용
        CashManager::getInstance()->acceptCash(insertedCash);
        std::cout << "Cash accepted. Fee of KRW " << fee << " has been deducted." << std::endl;

        // 계좌에 입금
        account->deposit(depositAmount);
        std::cout << "Deposited KRW " << depositAmount << " into account " << account->getAccountNumber() << "." << std::endl;

        // 트랜잭션 금액 설정
        this->amount = depositAmount;
    }
    else if (depositType == DepositType::CHECK) {
        int checkAmount = 0;
        while (true) {
            auto amountVariant = InputHandler::getInput("Please enter the check amount: ", InputType::INT);
            try {
                checkAmount = std::get<int>(amountVariant);
                if (checkAmount < 100000) {
                    std::cout << "Minimum check amount is KRW 100,000." << std::endl;
                    continue;
                }
                break;
            } catch (const std::bad_variant_access&) {
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }

        if (checkAmount < fee) {
            std::cout << "Check amount is insufficient to cover the fee of KRW " << fee << "." << std::endl;
            return false;
        }

        // 수수료 차감 후 실제 입금 금액 계산
        int depositAmount = checkAmount - fee;

        // 계좌에 입금
        account->deposit(depositAmount);
        std::cout << "Deposited KRW " << depositAmount << " into account " << account->getAccountNumber() << "." << std::endl;

        // 트랜잭션 금액 설정
        this->amount = depositAmount;
    }

    return true;
}

void DepositTransaction::rollback() {
    // Withdraw deposited amount
    try {
        if (account->withdraw(amount)) {
            std::cout << "Rollback: KRW " << amount << " has been withdrawn from account " << account->getAccountNumber() << "." << std::endl;
        } else {
            std::cout << "Rollback failed: Unable to withdraw deposited amount." << std::endl;
        }

        // Rollback fee is not necessary as fee cash has already been accepted
    } catch (const std::exception& e) {
        std::cout << "Error during rollback: " << e.what() << std::endl;
    }
}

void DepositTransaction::printDetails() const {
    std::cout << "Deposit Transaction [ID: " << transactionID << ", Amount: " << amount
              << ", Fee: " << fee << ", Account: " << account->getAccountNumber()
              << ", Type: " << (depositType == DepositType::CASH ? "Cash" : "Check") << "]" << std::endl;
}

