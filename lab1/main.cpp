#include <iostream>
#include <windows.h>

int main() {
    HANDLE commDev =  CreateFile("COM3",
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
        CloseHandle(commDev);
    }

    return 0;
}
