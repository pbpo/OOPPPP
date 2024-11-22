// BankManager.cpp
#include "BankManager.hpp"

BankManager* BankManager::instance = nullptr;

BankManager* BankManager::getInstance() {
    if (instance == nullptr) {
        instance = new BankManager();
    }
    return instance;
}

std::shared_ptr<Bank> BankManager::getBank(const std::string& bankName) {
    auto it = banks.find(bankName);
    if (it != banks.end()) {
        return it->second;
    } else {
        // 은행이 없으면 새로 추가
        addBank(bankName);
        return banks[bankName];
    }
}

void BankManager::addBank(const std::string& bankName) {
    banks[bankName] = std::make_shared<Bank>(bankName);
}

const std::map<std::string, std::shared_ptr<Bank>>& BankManager::getAllBanks() const {
    return banks;
}