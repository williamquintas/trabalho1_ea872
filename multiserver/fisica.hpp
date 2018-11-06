#ifndef FISICA_HPP
#define FISICA_HPP
#include "alvo.hpp"
#include "tiro.hpp"
#include "nave.hpp"
#include "audio.hpp"

#define VELOCIDADE_TIRO 10
#define ACELERACAO_TIRO 0
#define ALTURA_TELA 20
#define LARGURA_TELA 40
#define POSICAO_X_NAVE 3

class Fisica {
  private:
    Alvo *alvo;
    ListaDeNaves *lista_nave;
    ListaDeTiros *lista_tiro;
    Audio::Player *player;
    Audio::Sample *asample;
  public:
    Fisica(Alvo *alvo, ListaDeNaves *ldn, ListaDeTiros *ldt);
    void add_nave(Nave *n);
    void add_tiro(Tiro *t);
    void andar_nave(int deslocamento);
    void disparar_tiro(int i_tiro, int *total_tiros);
    void update_tiro(float deltaT, int *pontos);
    void destruir_tiro(int i_tiro);
};

#endif