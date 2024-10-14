#include <iostream>
#include <windows.h>
HANDLE commDev;
bool writeSerialPort(char buffer){
    unsigned long numberOfBytesWritten = 0;
    unsigned long numberOfBytesToWrite = 1;
    if(WriteFile(commDev, &buffer, numberOfBytesToWrite,
                 &numberOfBytesWritten, NULL)==0)
        return false;
    else
        return true;
}
bool readSerialPort(int numberOfBytesRead){
    int bytesRead;
    numberOfBytesRead=0;
    unsigned long numberOfBytesToRead=0;
    if (ReadFile (commDev, &bytesRead, 1, &numberOfBytesToRead, 0)){
        if (numberOfBytesToRead == 1){
            numberOfBytesRead = bytesRead;
            std::cout <<(char)numberOfBytesRead << std::endl;
            return true;
        }
    }
    return false;
}
int main() {
    std::string portName = "COM5";
    commDev =  CreateFile(portName.c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 NULL,
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

        char changeSettings;
        std::cout<<"Czy chcesz zmienic ustawienia portu? \n y/n?"<<std::endl;
        std::cin>>changeSettings;
        if(changeSettings == 'y'){
            dcb.BaudRate = CBR_1200; // prędkość transmisji
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
        ///timeout-y
        COMMTIMEOUTS timeouts;
        timeouts.ReadIntervalTimeout = 0;
        timeouts.ReadTotalTimeoutConstant = 5000;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 5000;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        if (SetCommTimeouts(commDev, &timeouts) == 0) {
            std::cout << "Blad wykonania funkcji SetCommTimeouts()"<<std::endl;
            CloseHandle(commDev);
            return false;
        }
        ///Wysłanie danych
//        char* inputChar = new char[256]{0};
//        char* buff = new char[256]{0};
//        std::cout<<"Podaj litere, ktora chcesz wyslac: ";
//        std::cin>>inputChar;
//        strcpy(buff,inputChar);
//        unsigned long bytesWritten = 0;
//        unsigned long bytesToWrite = 3;
//        if(WriteFile(commDev, buff, bytesToWrite, &bytesWritten, NULL) == 1){
//            std::cout<<"Wyslano: "<<buff<<std::endl;
//        }
        char charOut = 0;
        while((charOut = getchar()) != '\n'){
            std::cout<<charOut<<std::endl;
            writeSerialPort(charOut);
        }

        ///Odczyt danych
//        char* bytesRead = new char[256]{0};
//        unsigned long bytesToRead = 0;
//        if(ReadFile(commDev,bytesRead, 1, &bytesToRead, NULL)){
//            std::cout<<"Odebrano: "<<bytesRead<<std::endl;
//        }
        readSerialPort(3);
    }

    return 0;
}
