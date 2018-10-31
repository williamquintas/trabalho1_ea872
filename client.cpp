#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "oo_model.hpp"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>       /* time */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_TIROS 101
#define ALTURA_TELA 20
#define LARGURA_TELA 40

int socket_fd;

void *receber_respostas(void *parametros) {
  /* Recebendo resposta */
  char reply[60];
  int msg_len;
  int msg_num;
  msg_num = 0;
  while(1) {
    msg_len = recv(socket_fd, reply, 50, MSG_DONTWAIT);
    if (msg_len > 0) {
      struct ThreadArguments *ta = (struct ThreadArguments *) parametros;
      std::string buffer(sizeof(Nave), ' ');
      ta->nave->unserialize(buffer);
      ta->tela->update();
     // printw("[%d][%d] RECEBI:\n%s\n", msg_num, msg_len, reply);
      msg_num++;
    }
  }
}

int main() {
  struct sockaddr_in target;
  pthread_t receiver;
  struct ThreadArguments *ta = (struct ThreadArguments *) malloc(sizeof(struct ThreadArguments));
  //Criando os tiros
  ListaDeTiros *t_lista = new ListaDeTiros();
  for (int k=1; k<MAX_TIROS; k++){
    Tiro *tiro = new Tiro(0, 0, 0, 0);
    t_lista->add_tiro(tiro);
  }

  //Criando a Nave
  Nave *nave1 = new Nave(1);
  ListaDeNaves *n_lista = new ListaDeNaves();
  n_lista->add_nave(nave1);

  //Criando o alvo
  srand (time(NULL));
  Alvo *alvo = new Alvo((float)(rand() % (LARGURA_TELA/2) + LARGURA_TELA/2), (float)(rand() % ALTURA_TELA));
  Fisica *f = new Fisica(alvo, n_lista, t_lista);
  Tela *tela = new Tela(alvo, n_lista, t_lista, 20, 20, 20, 20);
  tela->init();
  tela->draw();
  Teclado *teclado = new Teclado();
  teclado->init();

  //Espera
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 // printw("Socket criado\n");
  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(target.sin_addr));
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
     // printw("Problemas na conexao\n");
      return 0;
  }
  //printw("Conectei ao servidor\n");
  ta->nave = nave1;
  ta->tela = tela;
  pthread_create(&receiver, NULL, receber_respostas, ta);

  /* Agora, meu socket funciona como um descritor de arquivo usual */
  while(1){
    char c = teclado->getchar();
    tela->update();
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
    if(c=='q'||c=='w'||c=='t'||c=='s'){
      send(socket_fd, &c, 1, 0);
     // printw("Escrevi mensagem de %c!\n",c);
      if (c=='q') {
        break;
      }
    }
   // std::this_thread::sleep_for (std::chrono::milliseconds(100));
  }

  //close(socket_fd);
  teclado->stop();
  tela->stop();
  return 0;
}
