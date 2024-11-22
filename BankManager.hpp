// BankManager.hpp
#pragma once
#include <memory>
#include <string>
#include <map>
#include "Bank.hpp"

class BankManager {
private:
    std::map<std::string, std::shared_ptr<Bank>> banks;

    BankManager() {}
    static BankManager* instance;

public:
    static BankManager* getInstance();

    std::shared_ptr<Bank> getBank(const std::string& bankName);
    void addBank(const std::string& bankName);
    const std::map<std::string, std::shared_ptr<Bank>>& getAllBanks() const;
};