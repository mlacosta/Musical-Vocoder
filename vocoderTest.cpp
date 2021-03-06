#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filtro.h"

void vocoderTest(float Coefficientes [CANTDEFILTROS][6][CANTDEETAPAS],float BufferAudio [BUFFERSIZE],float BufferSynth [BUFFERSIZE],float SenalProcesada [BUFFERSIZE],
             float Estado[2][CANTDEFILTROS][7][CANTDEETAPAS],float gain[CANTDEFILTROS],float CoefHilbert[ETAPASHILBERT],float EstadoHilbert[CANTDEFILTROS][ORDENHILBERT+2],
             float EstadoDelay [CANTDEFILTROS][ORDENDELAY+1],float CoefLPF[][ETAPASLPF],float EstadoLPF [CANTDEFILTROS][7][ETAPASLPF],float gainLPF ){

    FILE* BPF;
    //Lectura del Buffer Audio-------------------------------------------------------------------------
    FILE* bf = fopen("bufferAudio.dat","r");

    if (bf == NULL){
        printf("Open buffer audio failed. \n\n");
        return;
    }else{
        printf("Open buffer audio OK. \n\n");
        convertir(BufferAudio,bf,BUFFERSIZE);
        fclose(bf);
    }//----------------------------------------------------------------------------------------------

    //Lectura del Buffer Synth-------------------------------------------------------------------------
    bf = fopen("bufferSynth.dat","r");

    if (bf == NULL){
        printf("Open buffer synth failed. \n\n");
        return;
    }else{
        printf("Open buffer synth OK. \n\n");
        convertir(BufferSynth,bf,BUFFERSIZE);
        fclose(bf);
    }//----------------------------------------------------------------------------------------------

    //Lectura de los Filtros-------------------------------------------------------------------
    for(int m = 0; m<CANTDEFILTROS;m++){
        char nombreFiltro[20] = "BPF";
        char str[3];

        sprintf(str, "%d", m+1);

        strcat(nombreFiltro,str);
        strcat(nombreFiltro,".dat");
        BPF = fopen(nombreFiltro,"r");

        if (BPF == NULL){
            printf("\nOpen filter failed (%s). \n\n---------------------------\n\n",nombreFiltro);
            return;
        }else{
            printf("\nOpen filter OK (%s) \n\n",nombreFiltro);
            for(int k =0; k<6;k++){
                convertir(Coefficientes[m][k],BPF,CANTDEETAPAS);
            }
            fclose(BPF);
        }
    }//-------------------------------------------------------------------------------------------------

    //Lectura del Filtro Hilbert-------------------------------------------------------------------
    FILE* HILB = fopen("hilbert.dat","r");

    if (HILB == NULL){
        printf("\nOpen filter failed (hilbert.dat). \n\n---------------------------\n\n");
        return;
    }else{
        printf("\nOpen filter OK (hilbert.dat) \n\n");

        convertir(CoefHilbert,HILB,ETAPASHILBERT);

        fclose(HILB);
    }//-------------------------------------------------------------------------------------------------

    //Lectura del LPF-------------------------------------------------------------------
    FILE* LF;
    LF = fopen("LPF.dat","r");

    if (LF == NULL){
        printf("\nOpen filter failed (LPF.dat). \n\n---------------------------\n\n");
        return;
    }else{
        printf("\nOpen filter OK (LPF.dat) \n\n");

        for(int k =0; k<6;k++){
            convertir(CoefLPF[k],LF,ETAPASLPF);
        }


        fclose(LF);
    }//-------------------------------------------------------------------------------------------------



    //Filtrado de la senal--------------------------------------------------------------

    FILE* Resultado = fopen("resultadoVocoder.dat","w");

    if (Resultado == NULL){
        printf("Open resultado failed (%s). \n\n---------------------------\n\n","resultadoVocoder.dat");
        return;
    }else{
        printf("\nOpen resultado OK (%s). \n\n\n","resultadoVocoder.dat");

        vocoder(Coefficientes ,BufferAudio,BufferSynth ,SenalProcesada,Estado,gain, CoefHilbert,EstadoHilbert,EstadoDelay,CoefLPF,EstadoLPF,gainLPF) ;
        escribir(Resultado,SenalProcesada,BUFFERSIZE);
        printf("Vocoder Done. \n\n---------------------------\n\n");
        fclose(Resultado);
    }//-------------------------------------------------------------------------------------------------



}
