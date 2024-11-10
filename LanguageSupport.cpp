//
//  LanguageSupport.cpp
//  Termproject
//
//  Created by cho on 10/20/24.
//

#include "LanguageSupport.hpp"

LanguageSupport* LanguageSupport::instance = nullptr;

LanguageSupport::LanguageSupport() {
    currentLanguage = Language::ENGLISH;

    // Initialize messages
    messagesEnglish["welcome"] = "Welcome to the ATM!";
    messagesEnglish["enter_card"] = "Please enter your card number:";
    messagesEnglish["enter_password"] = "Please enter your password:";
    messagesEnglish["wrong_password"] = "Incorrect password.";
    messagesEnglish["card_invalid"] = "Invalid card.";
    messagesEnglish["session_end"] = "Session ended. Thank you!";
    messagesEnglish["select_language"] = "Please select a language: 1. English 2. Korean";
    messagesEnglish["select_transaction"] = "Please select a transaction type:";
    messagesEnglish["max_password_attempts"] = "Maximum password attempts exceeded. Session ending.";
    messagesEnglish["invalid_input"] = "Invalid input. Please try again.";
    messagesEnglish["invalid_choice"] = "Invalid choice.";

    messagesKorean["welcome"] = "ATM�� ���� ���� ȯ���մϴ�!";
    messagesKorean["enter_card"] = "ī�� ��ȣ�� �Է����ּ���:";
    messagesKorean["enter_password"] = "��й�ȣ�� �Է����ּ���:";
    messagesKorean["wrong_password"] = "��й�ȣ�� Ʋ�Ƚ��ϴ�.";
    messagesKorean["card_invalid"] = "��ȿ���� ���� ī���Դϴ�.";
    messagesKorean["session_end"] = "������ ����Ǿ����ϴ�. �����մϴ�!";
    messagesKorean["select_language"] = "�� �������ּ���: 1. ���� 2. �ѱ���";
    messagesKorean["select_transaction"] = "�ŷ� ������ �������ּ���:";
    messagesKorean["max_password_attempts"] = "��й�ȣ �Է� Ƚ���� �ʰ��߽��ϴ�. ������ �����մϴ�.";
    messagesKorean["invalid_input"] = "��ȿ���� ���� �Է��Դϴ�. �ٽ� �õ����ּ���.";
    messagesKorean["invalid_choice"] = "�߸��� �����Դϴ�.";
}

LanguageSupport* LanguageSupport::getInstance() {
    if (instance == nullptr) {
        instance = new LanguageSupport();
    }
    return instance;
}

void LanguageSupport::setLanguage(Language language) {
    currentLanguage = language;
}

std::string LanguageSupport::getMessage(const std::string& key) {
    if (currentLanguage == Language::ENGLISH) {
        return messagesEnglish[key];
    }
    else {
        return messagesKorean[key];
    }
}