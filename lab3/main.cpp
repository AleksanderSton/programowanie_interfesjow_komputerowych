#include <iostream>
#include <windows.h>
HANDLE commDev;
bool writeSerialPort(void* buffer, DWORD numberOfBytesToWrite){
    DWORD numberOfBytesWritten = 0;
    OVERLAPPED overlapped;
    if(WriteFile(commDev, buffer, numberOfBytesToWrite, &numberOfBytesWritten, 0) == 0){
        std::cout << "Błąd wysyłania danych!" << std::endl;
        return false;
    }
    else{
        std::cout<<"Wyslano: "<<(char*)buffer<<std::endl;
        return true;
    }

}
bool readSerialPort(void* buffer, DWORD numberOfBytesToRead){
    DWORD numberOfBytesRead=0;
    OVERLAPPED overlapped ;
    if (ReadFile (commDev, buffer, numberOfBytesToRead, &numberOfBytesRead, 0)){
        std::cout<<"Odebano: "<<(char*)buffer<<std::endl;
        return true;
    }
    return false;
}

int main() {
    std::string portName = "COM4";
    commDev =  CreateFile(portName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 0,
                                 NULL);
    if(commDev==INVALID_HANDLE_VALUE){
        std::cout<<"PORT NIE ZOSTAL OTWARTY DO TRANSMISJI!"<<std::endl<<std::endl;
    }
    else{
        std::cout<<"PORT ZOSTAL OTWARTY DO TRANSMISJI!"<<std::endl<<std::endl;
        DCB dcb;
        dcb.DCBlength = sizeof(dcb); // aktualny rozmiar struktury DCB
        GetCommState(commDev, &dcb); // udostępnienie aktualnych parametrów DCB
        std::cout<<"Poprzednie ustawienia:"<<std::endl;
        std::cout<<"\t- Predkosc transmisji: "<< dcb.BaudRate<<std::endl;
        std::cout<<"\t- Parzystosc: "<<dcb.fParity<<std::endl;
        std::cout<<"\t- Ustawienie parzystosci: "<<int(dcb.Parity)<<std::endl;
        std::cout<<"\t- Bity stopu: "<<dcb.StopBits<<std::endl;
        std::cout<<"\t- Bity danych: "<<int(dcb.ByteSize)<<std::endl;
        std::cout<<"\t- Kontrola lini DTR: "<<dcb.fDtrControl<<std::endl;
        ///timeout-y
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = 0;
        timeouts.ReadTotalTimeoutConstant = 5000;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 0;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        if (!SetCommTimeouts(commDev, &timeouts)) {
            std::cout << "Błąd ustawiania timeout'ów!" << std::endl;
            CloseHandle(commDev);
            return 1;
        }
        char changeSettings;
        std::cout<<"Czy chcesz zmienic ustawienia portu? \n y/n?"<<std::endl;
        std::cin>>changeSettings;
        if(changeSettings == 'y'){
            dcb.BaudRate = CBR_256000; // prędkość transmisji
            dcb.fParity = TRUE; // sprawdzanie parzystości
            dcb.Parity = NOPARITY; // ustawienie parzystości
            dcb.StopBits = TWOSTOPBITS; // bity stopu
            dcb.ByteSize = 7; // bity danych
            dcb.fDtrControl = 1; // np. kontrola linii DTR

            SetCommState(commDev,&dcb);
            GetCommState(commDev,  &dcb);
            std::cout<<"Ustawienia po zmianie:"<<std::endl;
            std::cout<<"\t- Predkosc transmisji: "<< dcb.BaudRate<<std::endl;
            std::cout<<"\t- Parzystosc: "<<dcb.fParity<<std::endl;
            std::cout<<"\t- Ustawienie parzystosci: "<<int(dcb.Parity)<<std::endl;
            std::cout<<"\t- Bity stopu: "<<dcb.StopBits<<std::endl;
            std::cout<<"\t- Bity danych: "<<int(dcb.ByteSize)<<std::endl;
            std::cout<<"\t- Kontrola lini DTR: "<<dcb.fDtrControl<<std::endl;
        }
        int choice;
        std::cout<<"Wybierz tryb wyslania"<<std::endl;
        std::cout<<"[1] Wyslanie litery"<<std::endl;
        std::cout<<"[2] Wyslanie lancucha znakow"<<std::endl;
        std::cin>>choice;
        if(choice == 1){
            ///Wysłanie liter
            char letter[2];
            std::cout<<"Podaj znak do wyslania: "<<std::endl;
            std::cin>>letter;
            letter[1] ='\0';
            char* buffIn = new char;
            strcpy(buffIn,letter);
            if(writeSerialPort(buffIn,2))
                std::cout<<"Pomyslnie wyslano litere!"<<std::endl;
            else
                std::cout<<"Blad w wyslaniu litery!"<<std::endl;
            ///Odczyt liter
            char *buffOut = new char;
            if(readSerialPort(buffOut,2))
                std::cout<<"Pomyslnie odebrano litere!"<<std::endl;
            else
                std::cout<<"Blad w odbiorze litery!"<<std::endl;
            delete buffIn;
            delete buffOut;
        }
        else if(choice == 2){
            ///Wysłanie tekstu
            std::string text;
            std::cout<<"Podaj teskt do wyslania!"<<std::endl;
            std::cin.ignore();
            getline(std::cin,text);
            int buffSize = text.size()+1;
            char* buffIn = new char[buffSize];
            strcpy(buffIn,text.c_str());
            if(writeSerialPort(buffIn,buffSize))
                std::cout<<"Pomyslnie wyslano tekst!"<<std::endl;
            else
                std::cout<<"Blad w wyslaniu tekstu!"<<std::endl;
            ///Odbieranie tekstu
            char* buffOut = new char[buffSize];
            if(readSerialPort(buffOut,buffSize))
                std::cout<<"Pomyslnie odebrano tekst!"<<std::endl;
            else
                std::cout<<"Blad w odbieraniu tekstu!"<<std::endl;
            delete buffIn;
            delete buffOut;
        }
    }
    CloseHandle(commDev);
    return 0;
}